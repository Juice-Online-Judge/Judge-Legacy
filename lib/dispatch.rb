#!/usr/bin/env ruby
#encoding:utf-8

require 'rubygems'
require 'bundler/setup'

require 'jimson'
require 'pathname'

require_relative 'pluginConfig'

class JudgeHandler
  extend Jimson::Handler
  C = 0
  CPP = 1
  def addJudge(type, pathStr)
    path = Pathname.new(pathStr)
    cmd = ""
    res = "AC"
    case type
    when C
      name = path.basename(".c")
      cmd = "gcc -O2 -o #{path.dirname + name} #{pathStr} -lm"
    when CPP
      name = path.basename(".cpp")
      cmd = "g++ -O2 -o #{name} #{path.basename} -lm"
    else
      cmd = ExtraCmdGet::cmdGet(path.extname, pathStr)
      return "Error:Unknown file type" unless cmd
    end
    res = "CE" unless system cmd
    return res
  end
end

server = Jimson::Server.new(JudgeHandler.new, {host:"127.0.0.1", port:4242})
server.start
