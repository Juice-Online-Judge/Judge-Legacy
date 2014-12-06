#!/usr/bin/env ruby
#encoding:utf-8

require_relative 'database'

class UserSubmission < Sequel::Model(:user_submission)
  many_to_one :lesson_exercise, key: :link_id
end
