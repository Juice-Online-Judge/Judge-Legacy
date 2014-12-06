#!/usr/bin/env ruby
#encoding:utf-8

path = File.dirname(__FILE__)
selfName = File.basename(__FILE__)

Dir.glob(File.join(path, "*")).map{ |name| File.basename(name)}.each { |name|
  require_relative name unless name == selfName or name == "database.rb"
}
