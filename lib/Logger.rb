#!/usr/bin/env ruby
#encoding:utf-8

require 'rubygems'
require 'logger'

require '../config/environment'

f = File.new("#{AppPath}/run/Dispath.log", "a")
f.sync = true
$logger = Logger.new f
