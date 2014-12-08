#!/usr/bin/env ruby
# encoding: UTF-8

require 'model/spec_helper'
require 'model/model'

describe UserSubmission do
  it "should save with valid data" do
    submission = FactoryGirl.build(:user_submission)
    submission.save
    expect(submission.id).to eq(1)
    expect(submission.submit_type).to eq(1)
    expect(submission.lesson_exercise.id).to eq(1)
  end

  it "can multi association with one lession_exercise" do
    submission1 = FactoryGirl.create(:user_submission)
    expect(submission1.id).to eq(1)
    expect(submission1.submit_type).to eq(1)
    expect(submission1.lesson_exercise.id).to eq(1)
    submission2 = FactoryGirl.create(:user_submission, lesson_exercise: submission1.lesson_exercise)
    expect(submission2.id).to eq(2)
    expect(submission2.submit_type).to eq(1)
    expect(submission2.lesson_exercise.id).to eq(1)
  end
end
