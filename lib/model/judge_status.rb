#!/usr/bin/env ruby
#encoding:utf-8

require 'rubygems'

require_relative '../../config/environment.rb'

require 'sqlite3'
require 'active_record'

ActiveRecord::Base.establish_connection(adapter:"sqlite3", database:"#{AppPath}/run/judge.db")

class Lesson_Implement < ActiveRecord::Base
  self.table_name = "lesson_implement"
end

class User_Code_Lesson < ActiveRecord::Base
  self.table_name = "user_code_lesson"
end
