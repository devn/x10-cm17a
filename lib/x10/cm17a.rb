#!/usr/bin/env ruby

require 'x10'
require 'cm17a_api'
require 'x10/cm17a_device'

module X10

  module Cm17a

    # The Controller object is the low level interface to the CM17A
    # Firecracker controller. Client software generally uses the
    # device level interface rather than the controller directly.
    class Controller

      # Create an X10::Cm17a::Device on this controller at the given
      # X10 house and unit address.
      def device(house, unit)
        X10::Cm17a::Device.new(house, unit, self)
      end

      # Yes, this class represents a X10 controller.
      def self.x10_controller?
        true
      end
    end

  end
end
