#!/usr/bin/env ruby
#encoding:utf-8

module LangType
  C = 0
  CPP = 1
end

Result = {
  "AC" => 1,
  "CE" => 2,
  "WA" => 3,
  "TLE" => 4,
  "MLE" => 5,
  "RE" => 6
}

module Mode
  Diff = 0
  WS = 1
  BLine = 2
  BLineWS = 3
  Loader = 4
end
