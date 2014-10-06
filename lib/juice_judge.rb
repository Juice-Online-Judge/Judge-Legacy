#!/usr/bin/env ruby
#encoding:utf-8

require 'active_record'
require 'jsonrpc-client'

client = JSONRPC::Client.new("http://localhost:4242")

client.addJudge(ARGV[0], ARGV[1], ARGV[2]) if ARGV[0] and ARGV[1] and ARGV[2]
