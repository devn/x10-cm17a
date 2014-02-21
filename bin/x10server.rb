require 'drb'
require 'x10/cm17a'
require 'optparse'

uri = 'druby://:7777'

opts = OptionParser.new do |opts|
  opts.on('-h', '--help', 'Display this help message') do
    puts opts
    exit
  end
  opts.on('-p', '--port=PORT', 'Use PORT as the connection port.') do |port|
    uri = "druby://:#{port}"
  end
end

opts.parse!(ARGV)

DRb.start_service(uri, X10::Cm17a::Controller.new)
DRb.thread.join 
