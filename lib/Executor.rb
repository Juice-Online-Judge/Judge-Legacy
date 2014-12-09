#!/usr/bin/env ruby
#encoding:utf-8

require 'rubygems'

require_relative File.join(File.dirname(__FILE__), "..", "config", "environment") unless defined? AppPath

require 'ffi'

module Executor
  extend FFI::Library
  ffi_lib "#{AppPath}/lib/executor/libexecutor.so"
  Pass = 0
  TLE = 1
  MLE = 2
  RE = 3
  attach_function :executor, [:string, :string, :string, :int, :int], :int
  def self.execute(ques, path, tle, mle)
    self.executor(AppPath, ques, path, tle, mle)
  end
end
