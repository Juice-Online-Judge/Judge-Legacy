#!/usr/bin/env ruby
#encoding:utf-8

require 'rubygems'
require 'thread'
require 'open3'
require 'pathname'

require_relative '../config/environment.rb'
require_relative 'model/judge_status.rb'
require_relative 'Const'
require_relative 'pluginConfig'

require 'jimson'

taskQueue = Queue.new

Thread.new {
  while (task = taskQueue.pop)
    codeKey = task[:codeKey]
    data = User_Code_Lesson.where(code_key: codeKey).first
    code = data.user_code
    cmd = ""
    res = "AC"
    type = LangType.C
    case type
    when LangType.C
      name = path.basename(".c")
      cmd = "gcc -O2 -o #{codeKey} -xc - -lm"
    when LangType.CPP
      name = path.basename(".cpp")
      cmd = "g++ -O2 -o #{codeKey} -xc - -lm"
    else
      cmd = ExtraCmdGet::cmdGet(path.extname, pathStr)
      return "Error:Unknown file type" unless cmd
    end
    _, s = Open3.capture2(cmd, stdin_data: code)
    res = "CE" unless s.exitstatus == 0
    data.result = Result[res]
    data.save
  end
}

class JudgeHandler
  extend Jimson::Handler
  def addJudge(codeKey, tableName, problemKey)
    taskQueue << {codeKey:codeKey, tableName:tableName, problemKey:problemKey}
  end
end

server = Jimson::Server.new(JudgeHandler.new, {host:"127.0.0.1", port:4242})
server.start
