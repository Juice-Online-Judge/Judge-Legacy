#!/usr/bin/env ruby
#encoding:utf-8

require 'model/spec_helper'
require 'model/model'

describe LessonExercise do
  it "should save valid data" do
    lesson = FactoryGirl.create(:lesson_exercise)
    expect(lesson.id).to eq(1)
    expect(lesson.lesson_unit).to eq(0)
    expect(lesson.exercise_answer).to eq(nil)
  end

  it ".lesson_unit should not be nil" do
    expect {
      FactoryGirl.create(:lesson_exercise, lesson_unit: nil)
    }.to raise_error(Sequel::NotNullConstraintViolation)
  end
end
