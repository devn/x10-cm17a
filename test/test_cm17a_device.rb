#!/usr/bin/env ruby

begin
  require 'rubygems'
rescue LoadError
  # nothing
end

require 'test/unit'
require 'flexmock'
require 'x10/cm17a'

######################################################################
class NoopClass
  def method_missing(sym, *args, &block)
    self
  end
end

NOOP = NoopClass.new

######################################################################
class MockController < FlexMock
  class << self
    def setup_controller(controller)
      X10.controller =  controller
      controller.mock_handle(:device, 1) { |house, unit|
	X10::Cm17a::Device.new(house, unit, controller)
      }
    end
    
    def use
      super do |mock|
	setup_controller(mock)
	yield(mock)
      end
    end
  end
end

######################################################################
class TestCm17a < Test::Unit::TestCase
  def setup
  end

  def teardown
    X10.controller = nil
  end

  def test_create
    MockController.use do |controller|
      dev = X10.device('a1')
      assert_not_nil(dev)
    end
  end

  def test_on
    MockController.use do |controller|
      controller.mock_handle(:command, 1) { |house, unit, cmd, step|
	assert_equal :on, cmd
	assert_equal 0, step
      }
      dev = X10.device('a1')
      dev.on
    end
  end

  def test_off
    MockController.use do |controller|
      controller.mock_handle(:command, 1) { |house, unit, cmd, step|
	assert_equal :off, cmd
	assert_equal 0, step
      }
      dev = X10.device('a1')
      dev.off
    end
  end

  def test_adjust
    MockController.use do |controller|
      controller.mock_handle(:command, 2) { |house, unit, cmd, step|
	if step == 4
	  assert_equal 4, step
	  assert_equal :brighten, cmd
	else
	  assert_equal 3, step
	  assert_equal :dim, cmd
	end
      }
      dev = X10.device('a1')
      dev.step(4)
      dev.step(-3)
    end
  end

  def test_big_adjustments
    MockController.use do |controller|
      controller.mock_handle(:command, 2) { |house, unit, cmd, step|
	assert_equal :dim, cmd
	if step == 6
	  assert_equal 6, step
	else
	  assert_equal 3, step
	end
      }
      dev = X10.device('a1')
      dev.step(-9)
    end
  end

  def test_small_adjustments
    MockController.use do |controller|
      controller.mock_handle(:command, 0) { }
      dev = X10.device('a1')
      dev.step(0)
    end
  end

  def test_device_addressing
    MockController.use do |controller|
      controller.mock_handle(:command) { |house, unit, cmd, step|
	assert_equal 1, house
	assert_equal 2, unit
      }
      dev = X10.device('b3')
      dev.off
      dev.on
      dev.step(1)
      dev.step(-1)
    end
  end
end

######################################################################
class TestCm17aController < Test::Unit::TestCase
  def test_create
    X10.controller = X10::Cm17a::Controller.new
    d = X10.device('a1')
    assert_not_nil d
  end
end
