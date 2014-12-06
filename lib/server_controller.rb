#!/usr/bin/env ruby
#encoding:utf-8

require_relative '../config/environment'

require 'daemons'

Daemons.run(File.join(AppPath, "lib", "server.rb"), dir: "../run/lock", dir_mode: :normal)
