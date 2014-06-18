#!/usr/bin/env ruby
#encoding:utf-8

require 'rubygems'
gem 'bundler'

require 'yaml'

data = File.read(File.join(File.dirname(__FILE__), "config.yml"))

config = YAML.load(data)

p config
