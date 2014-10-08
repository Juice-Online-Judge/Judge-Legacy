#!/usr/bin/env ruby
#encoding:utf-8

require 'rubygems'
require 'thread'
require 'open3'
require 'pathname'

require_relative '../config/environment.rb'
require_relative 'model/judge_status.rb'
require_relative 'Const'
require_relative 'Logger'
require_relative 'Executor'
require_relative 'judge'
require_relative 'pluginConfig'

require 'jimson'

$taskQueue = Queue.new

Thread.new {
  while (task = $taskQueue.pop)
    begin
      $logger.info "Worker get task"
      codeKey = task[:codeKey]
      data = User_Code_Lesson.where(code_key: codeKey).first
      unless data
        $logger.error "Could not get data for key:#{codeKey}"
      end
      code = data.user_code
      cmd = ""
      res = "AC"
      type = LangType::C
      case type
      when LangType::C
        cmd = "gcc -O2 -include #{AppPath}/lib/define.h -o #{AppPath}/run/exe/#{codeKey} -xc - -lm"
      when LangType::CPP
        cmd = "g++ -O2 -include #{AppPath}/lib/define.h -o #{codeKey} -xc - -lm"
      else
        # cmd = ExtraCmdGet::cmdGet(path.extname, pathStr)
        return "Error:Unknown file type" unless cmd
      end
      _, s = Open3.capture2(cmd, stdin_data: code)
      if s.exitstatus == 0
        $logger.info "Start judge #{codeKey}"
        quesData = Lesson_Implement.where(implement_key: data.ipm_pt_key).first
        ret = Executor.executor(data.ipm_pt_key, File.join(AppPath, "run", "exe", codeKey), quesData.time_limit, quesData.memory_limit)
        res = ReturnCode[ret] if res != 0
        if res == "AC"
          res = "WA" unless Judger.judge(quesData.ipm_pt_key, File.read(File.join(AppPath, "run", "ans", codeKey + ".ans")), quesData.mode)
        end
      else
        $logger.info "Code: #{codeKey} compile error"
        res = "CE"
      end
      data = User_Code_Lesson.find(data.id)
      data.result = Result[res]
      data.save
    rescue Exception => e
      $logger.error e.to_s
    end
  end
}

class JudgeHandler
  extend Jimson::Handler
  def addJudge(codeKey, tableName, problemKey)
    begin
      $logger.info "Add work key:#{codeKey}"
      $taskQueue << {codeKey:codeKey, tableName:tableName, problemKey:problemKey}
    rescue Exception => e
      $logger.error e.to_s
      throw
    end
  end
end

server = Jimson::Server.new(JudgeHandler.new, {host:"127.0.0.1", port:4242})
server.start
