#!/usr/bin/env ruby

require 'test/unit'
require 'x10/cm17a'
require 'rbconfig'

# There's not much we can test on the api code without assuming a
# bunch of hardware.  The output of the API is a sequence of on/off
# states in the DTS/RTS RS-232 signals.  See the file
# cm17a_protocol.txt in the doc directory for details.
#
class TestApi < Test::Unit::TestCase
  DEV = (Config::CONFIG['arch'] =~ /win32/) ? "COM1" : "/dev/ttyS0"

  def teardown
    X10.controller.close if X10.controller
    X10.controller = nil
  end

  def test_default_create
    cm = X10::Cm17a::Controller.new
    assert_not_nil cm
  end

  def test_specific_create
    cm = X10::Cm17a::Controller.new(DEV)
    assert_not_nil cm
  end

  def test_bad_create
    assert_raises(X10::X10Error) {
      cm = X10::Cm17a::Controller.new("/dev/bad device")
    }
  end
end
