#!/usr/bin/env ruby
#encoding:utf-8

require 'rubygems'
require_relative '../config/environment.rb'

class ExtraCmdGet
  @@langSupport = {}
  def self.cmdGet(ext, path)
    if @@langSupport.key?(ext)
      return self.send(@@langSupport[ext] + "Cmd", path)
    else
      return nil
    end
  end
end

list = Dir.glob("./plugin/**/*.rb")
list.each { |x|
  require_relative x
}
