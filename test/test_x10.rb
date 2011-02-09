#!/usr/bin/env ruby

require 'test/unit'

class TestX10 < Test::Unit::TestCase
  def test_parse_name
    assert_equal [0, 0], X10.parse_address("a1")
    assert_equal [0, 0], X10.parse_address("A1")
    assert_equal [1, 0], X10.parse_address("b1")
    assert_equal [0, 1], X10.parse_address("a2")
    assert_equal [7, 5], X10.parse_address("h6")
    assert_equal [7, 5], X10.parse_address("H6")
    assert_equal [15, 15], X10.parse_address("p16")
    assert_equal [15, 15], X10.parse_address("P16")
  end

  def test_bad_house
    assert_raises(X10::X10Error) { X10.parse_address("11") }
    assert_raises(X10::X10Error) { X10.parse_address("?1") }
  end

  def test_bad_format
    assert_raises(X10::X10Error) { X10.parse_address("") }
    assert_raises(X10::X10Error) { X10.parse_address("abcd12") }
  end

  def test_bad_unit
    assert_raises(X10::X10Error) { X10.parse_address("a0") }
    assert_raises(X10::X10Error) { X10.parse_address("a17") }
  end

  def test_reverse_name
    assert_equal "b10", X10.make_address(1, 9)
  end
end

