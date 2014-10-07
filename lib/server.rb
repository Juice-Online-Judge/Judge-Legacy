#!/usr/bin/env ruby
#encoding:utf-8

require_relative '../config/environment'
require_relative 'Logger'

if not File.exists?(File.join(AppPath, "lib", "executor", "libexecutor.so"))
  $logger.error "Could not found extension"
  puts "Please build the extension first"
  exit 1
end

$logger.info "Server start"
require_relative 'dispatch'
