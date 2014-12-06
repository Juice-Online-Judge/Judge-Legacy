#!/usr/bin/env ruby
#encoding:utf-8

require 'uri'
require 'jsonrpc-client'

client = JSONRPC::Client.new("http://localhost:4242/rpc")

client.addJudge(ARGV[0]) if ARGV[0]
