#!/usr/bin/env ruby

# = X10 Interface
#
# This is a really simple X10 framework for controlling X10 devices.
#
# == Example:
#
#   require 'x10/cm17a'
#   lamp = X10.device("a1")
#   lamp.on
#   sleep(1)
#   lamp.off
#
# == Controllers and Devices
#
# An X10 controller is responsible for interfacing to the X10
# interface device.  An example is the CM17A firecracker module.
#
# == Default Controllers
#
# blah blah blah.
#
class Class			# :nodoc:
  def x10_controller?
    false
  end
end

# The X10 Module.  This module provides a root namespace for all X10
# devices and software.  It also provides a few utility methods for
# use by the X10 controllers and devices.
module X10
  
  # Error thrown for X10 specific failures.
  class X10Error < RuntimeError; end

  class << self
    # Create an X10 device at the given address.  THe address should
    # be a string specifying the device address is standard X10
    # nomenclature (e.g. a1 ... a16, b1 ... b16, ... p1 ... p16).
    def device(address)
      house, unit = parse_address(address)
      controller.device(house, unit)
    end
    
    # Return the controller currently in use.  
    def controller
      @controller ||= discover_single_controller
    end
    
    # Set the controller to be used to create X10 devices.  If there
    # is only one X10 controller loaded, then that controller will be
    # used by default.  Otherwise, the controller must be explicitly
    # set using this method.
    def controller=(controller)
      @controller = controller
    end
    
    # Make a canonical X10 device address from the house number and
    # unit.  House and unit numbers are zero based.
    def make_address(house, unit)
      (house.ord + ?a.ord).chr + (unit+1).to_s
    end

    # Parse a canonical X10 device adderss into house number and unit
    # number.  House and unit numbers are zero based.
    def parse_address(address)
      address = address.downcase
      if address !~ /^([a-p])(\d+)$/
	fail X10::X10Error, "Bad X10 device address [#{address}]"
      end
      house_letter = $1
      unit = $2.to_i - 1
      
      if unit < 0 || unit > 15
	fail X10::X10Error, "Bad X10 device address [#{address}]"
      end
      
      house = address[0].ord - ?a.ord
      [house, unit]
    end

    # If there is only one X10 controller class defined in the object
    # space, then use it by default.
    def discover_single_controller
      controllers = []
      ObjectSpace.each_object(Class) do |c|
	controllers << c if c.x10_controller?
      end
      case controllers.size
      when 0
	fail X10::X10Error, "No X10 Controllers Found"
      when 1
	controllers.first.new
      else
	fail X10::X10Error, "Multiple X10 Controllers Found"
      end      
    end
  end

end
