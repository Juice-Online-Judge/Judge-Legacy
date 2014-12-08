#!/usr/bin/env ruby
# encoding: UTF-8

FactoryGirl.define do
  to_create {|i| i.save}
  factory :lesson_exercise do
    lesson_unit 0
    exercise_answer nil
  end

  factory :user_submission do
    lesson_exercise
    submit_type 1
    submit_content "#include <stdio.h>\nint main() {\n}"
  end
end
