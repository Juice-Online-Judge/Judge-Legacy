#!/usr/bin/env ruby
#encoding:utf-8

require_relative File.join(File.dirname(__FILE__), "..", "config", "environment") unless defined? AppPath
require_relative 'Logger'

if not File.exists?(File.join(AppPath, "lib", "executor", "libexecutor.so"))
  $logger.error "Could not found extension"
  puts "Please build the extension first"
  exit 1
end

File.write(File.join(AppPath, "run", "lock", "server.pid"), Process.pid)

$logger.info "Server start"
require_relative 'dispatch'
File.unlink(File.join(AppPath, "run", "lock", "server.pid"))
