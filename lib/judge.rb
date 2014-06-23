#!/usr/bin/env ruby
#encoding:utf-8

require 'rubygems'

require_relative '../config/environment.rb'

class Judger
  def Judget.judge(ques, ans)
    out = File.read(File.join(AppPath, "run", "out", "#{ques}.out"))
    out.each_line.zip(ans.each_line) { |x, y|
      if x != y
        return false
      end
    }
    return true
  end
end
