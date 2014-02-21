#!/usr/bin/env ruby

require 'drb'
require 'x10'
require 'x10/cm17a_device'

module X10
  module Cm17aRemote
    class Controller
      def initialize(uri="druby:localhost:7777")
        @uri = uri
        @remote = DRbObject.new(nil, @uri)
      end
      def command(house, unit, command, steps)
        @remote.command(house, unit, command, steps)
      end
      def device(house, unit)
        X10::Cm17a::Device.new(house, unit, self)
      end
      def self.x10_controller?
        true
      end
    end
  end
end
