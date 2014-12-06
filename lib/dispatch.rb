#!/usr/bin/env ruby
#encoding:utf-8

require 'rubygems'
require 'thread'
require 'open3'
require 'pathname'

require_relative '../config/environment.rb'
require_relative 'model/model'
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
      $logger.info "Fetching user submission data"
      data = UserSubmission.where(id: codeKey).first
      unless data
        raise "Could not get data for key:#{codeKey}"
      end
      $logger.info "Fetch done"
      code = data.submit_content
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
        quesData = data.lesson_exercise
        $logger.debug "Before exec res: #{res}"
        ret = Executor.executor(codeKey, File.join(AppPath, "run", "exe", codeKey), quesData.exercise_tle, quesData.exercise_mle)
        $logger.debug "After exec res: #{res}"
        $logger.debug "After exec return code: #{ret}"
        res = ReturnCode[ret] if ret != 0
        if res == "AC"
          res = "WA" unless Judger.judge(codeKey, File.read(File.join(AppPath, "run", "ans", quesData.id.to_s + ".ans")), quesData.exercise_judge_mode)
        end
      else
        $logger.info "Code: #{codeKey} compile error"
        res = "CE"
      end
      data.judge_result = Result.fetch(res, 6)
      data.save
    rescue RuntimeError => e
      $logger.error e.to_s
    rescue Exception => e
      $logger.error e.to_s
      $logger.error e.backtrace
    end
  end
}

class JudgeHandler
  extend Jimson::Handler
  def addJudge(codeKey)
    begin
      $logger.info "Add work key:#{codeKey}"
      $taskQueue << {codeKey:codeKey}
    rescue Exception => e
      $logger.error e.to_s
      throw
    end
  end
end

server = Jimson::Server.new(JudgeHandler.new, {host:"127.0.0.1", port:4242})
server.start
