#!/usr/bin/env ruby
#encoding:utf-8

require 'rubygems'

require_relative '../config/environment.rb'

class Judger
  def Judger.judge(ques, ans, mode)
    out = File.read(File.join(AppPath, "run", "out", "#{ques}.out"))
    out.each_line.zip(ans.each_line) { |x, y|
      case mode
      when Mode::Diff
        return false if x != y
      when Mode::WS
        return false if x.strip != y.strip
      when Mode::BLine
        return false if x != y and x != "" and y != ""
      when Mode::BLineWS
        return false if x.strip != y.strip and x != "" and y != ""
      end
    }
    return true
  end
end
