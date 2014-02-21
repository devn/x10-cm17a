#!/usr/bin/env ruby

module X10

  module Cm17a

    # A CM17A device that responds to on/off and adjust (brightness)
    # commands.
    #
    # Typical use:
    #
    #  lamp = X10.device("a2")   # Create the device
    #  lamp.on                   # Turn the lamp on
    #  lamp.adjust(-0.5)         # Dim the lamp
    #  lamp.off                  # Turn the lamp off
    #
    class Device
      attr_reader :address, :controller

      # Create a new X10 device using a CM17A protocol controller.
      # Such a device can handle on/off commands and simple dim
      # controls.
      #
      # Normally device objects are created through the X10 framework
      # and not directly by the client software.
      def initialize(house, unit, controller)
        @house = house
        @unit = unit
        @controller = controller
        @address = X10.make_address(house, unit)
      end

      # Turn the device on.
      def on
        @controller.command(@house, @unit, :on, 0)
      end

      # Turn the device off.
      def off
        @controller.command(@house, @unit, :off, 0)
      end

      # Adjust the brightness level by +steps+.  If +steps+ is
      # positive, the brightness level is increased that many steps.
      # If +steps+ is negative, the brightness level is decreased by
      # <tt>|steps|</tt>.
      #
      # The controller limits the step size to 6, so larger steps must
      # be broken up.
      def step(steps)
        direction = (steps > 0) ? :brighten : :dim
        nsteps = steps.abs
        while nsteps > 0
          n = (nsteps > 6) ? 6 : nsteps
          @controller.command(@house, @unit, direction, n)
          nsteps -= n
        end
      end
    end
  end
end
