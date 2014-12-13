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
  attach_function :executorInit, [:string], :void
  attach_function :executor, [:string, :string, :int, :int], :int
  executorInit(AppPath)
  def self.execute(ques, path, tle, mle)
    self.executor(ques, path, tle, mle)
  end
end
