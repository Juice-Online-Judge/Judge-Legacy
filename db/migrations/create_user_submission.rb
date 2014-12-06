#!/usr/bin/env ruby
#encoding:utf-8

Sequel.migration do
  change do
    create_table(:user_submission) do
      primary_key :id
      Integer :link_id, null: false
      Integer :submit_type, null:false
      String :submit_content, text: true
      Integer :judge_result, default: -1
      Integer :usage_mem
      Integer :usage_time
      Integer :usage_file
      Integer :submit_user
      Integer :submit_time
      String :submit_tp, text: true
    end
  end
end
