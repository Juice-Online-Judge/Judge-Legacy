#!/usr/bin/env ruby
#encoding:utf-8

require 'rubygems'

require_relative '../config/environment.rb'

require 'ffi'

module Executor
  extend FFI::Library
  ffi_lib "executor/libexecutor.so"
  Pass = 0
  TLE = 1
  MLE = 2
  RE = 3
  attach_function :executor, [:string, :string, :int, :int], :int
end
