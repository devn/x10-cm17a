#!/usr/bin/env ruby

require 'optparse'
require 'open-uri'
require 'x10/cm17a'

opts = OptionParser.new do |opts|
  opts.on_tail("-h", "--help", "Display this help message.") do
    puts opts
    exit
  end
  opts.on("-b", "--bad=X10ADDR", "X10 address of the 'bad' device") do |value|
    $bad_address = value
  end
  opts.on("-g", "--good=X10ADDR", "X10 address of the 'good' device") do |value|
    $good_address = value
  end
  opts.on("-t", "--timeout=SECONDS", Float, "Seconds between updates") do |value|
    $timeout = value
  end
  opts.on("-u", "--url", "URL of RubyGems Build Monitor Page") do |value|
    $url = value
  end
end

$url = "http://onestepback.org/cgi-bin/rubygems_build.cgi"
$timeout = 60
$good_address = 'a2'
$bad_address = 'a3'

opts.parse!(ARGV)

good = X10.device($good_address)
bad  = X10.device($bad_address)

loop do
  begin
    open($url) do |f|
      data = f.read
      if /Build Status:\s*OK/ =~ data
	puts "Build is OK"
	good.on
	bad.off
      elsif /Build Scheduled/ =~ data
	puts "Build Scheduled"
	good.off
	bad.off
      elsif /Build In Progress/ =~ data
	puts "Build in Progress"
	good.on
	bad.on
      else
	puts "Build is broken"
	good.off
	bad.on
      end
    end
  rescue Timeout::Error => ex
    puts "Web page timed out: #{ex.message}"
  end
  sleep($timeout)
end

