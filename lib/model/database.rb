#!/usr/bin/env ruby
#encoding:utf-8

require 'rubygems'

require_relative '../../config/environment.rb'

require 'sequel'

Object.send(:remove_const, :DB) if defined? DB
DB = Sequel.connect($database[Environment])
Sequel::Model.db = DB
