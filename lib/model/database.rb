#!/usr/bin/env ruby
#encoding:utf-8

require 'rubygems'

require_relative '../../config/environment.rb'

require 'sequel'

Sequel.connect($database[$config[:mode]])
