#!/usr/bin/env ruby
#encoding:utf-8

class ExtraCmdGet
  @@langSupport[".java"]="java"
  def self.javaCmd(path)
    return "javac -d #{File.dirname(path)} #{path}"
  end
end
