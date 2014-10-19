#!/usr/bin/env ruby
#encoding:utf-8

require 'rubygems'

require_relative '../../config/environment.rb'

require 'sequel'

Sequel.connect($database[$config[:mode]])

class Lesson_Implement < Sequel::Model(:lesson_implement)
  set_primary_key :id
end

class User_Code_Lesson < Sequel::Model(:user_code_lesson)
  set_primary_key :id
end
