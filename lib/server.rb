#!/usr/bin/env ruby
#encoding:utf-8

require_relative '../config/environment'

if not File.exists?(File.join(AppPath, "lib", "executor", "libexecutor.so"))
  puts "Please build the extension first"
  exit 1
end

require_relative 'dispatch'
