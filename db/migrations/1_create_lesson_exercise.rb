#!/usr/bin/env ruby
#encoding:utf-8

Sequel.migration do
  change do
    create_table(:lesson_exercise) do
      primary_key :id
      Integer :lesson_unit, null: false
      String :exercise_title
      String :exercise_content, text: true
      Integer :exercise_answer
      Integer :exercise_tle
      Integer :exercise_mle
      Integer :exercise_fle
      Integer :exercise_judge_mode
      Integer :exercise_judge_added
      Integer :exercise_is_visble, default: 1
      Integer :exercise_is_delete, default: 0
    end
  end
end
