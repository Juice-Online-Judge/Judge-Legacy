#!/usr/bin/env ruby
#encoding:utf-8

require 'rubygems'
require 'logger'

require_relative File.join(File.dirname(__FILE__), "..", "config", "environment") unless defined? AppPath

f = File.new("#{AppPath}/run/Dispath.log", "a")
f.sync = true
$logger = Logger.new f
