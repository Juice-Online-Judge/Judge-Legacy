#!/usr/bin/env ruby
#encoding:utf-8

require 'rubygems'

require_relative '../config/environment.rb'

require 'ffi'

module Executor
  extend FFI::Library
  ffi_lib "executor/libexecutor.so"
  attach_function :executor, [:string, :string, :int, :int], :int
end
