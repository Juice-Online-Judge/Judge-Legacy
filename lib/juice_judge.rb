#!/usr/bin/env ruby
#encoding:utf-8

require 'active_record'
require 'jsonrpc-client'

client = JSONRPC::Client.new("http://localhost:4242")

