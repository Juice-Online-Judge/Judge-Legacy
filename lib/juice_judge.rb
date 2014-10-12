#!/usr/bin/env ruby
#encoding:utf-8

require 'uri'
require 'jsonrpc-client'

client = JSONRPC::Client.new("http://localhost:4242/rpc")

client.addJudge(ARGV[0], ARGV[1], ARGV[2]) if ARGV[0] and ARGV[1] and ARGV[2]
