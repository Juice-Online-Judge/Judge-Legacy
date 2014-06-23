#!/usr/bin/env ruby
#encoding:utf-8

require 'rubygems'

require_relative '../../config/environment.rb'

require 'sqlite3'
require 'active_record'

ActiveRecord::Base.establish_connection(adapter:"sqlite3", database:"#{AppPath}/run/judge.db")

class Judge_Competition_Result < ActiveRecord::Base
end

p Judge_Competition_Result.all
