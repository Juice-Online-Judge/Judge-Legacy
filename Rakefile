require 'fileutils'

def which(cmd)
  exts = ENV['PATHEXT'] ? ENV['PATHEXT'].split(';') : ['']
  ENV['PATH'].split(File::PATH_SEPARATOR).each do |path|
    exts.each { |ext|
      exe = File.join(path, "#{cmd}#{ext}")
      return exe if File.executable?(exe) && !File.directory?(exe)
    }
  end
  return nil
end

directory "run/in"
directory "run/out"
directory "run/exe"
directory "run/ans"
directory "bin"
directory "lib/executor"

task default: %w(build db:migrate)

desc "Make necessary directory"
task :fileStruct => %w(run/in run/out run/exe run/ans bin lib/executor)

desc "Build extenstion and file structure"
task :build => [:fileStruct] do
  verbose(false) do
    if which("scons")
      sh "scons"
      FileUtils.mv("build/executor", "bin")
      FileUtils.mv("build/libexecutor.so", "lib/executor")
    else
      raise "Please install scons"
    end
  end
end

task :clean do
  rm_rf "build"
end

namespace :db do
  require "sequel"
  Sequel.extension :migration
  DB = Sequel.connect("sqlite://db/judge.db")

  desc "Perform migration up to latest migration available"
  task :migrate do
    Sequel::Migrator.run(DB, "db/migrations")
  end

  desc "Perform rollback to specified target or full rollback as default"
  task :rollback, :target do |t, args|
    args.with_defaults(:target => 0)

    Sequel::Migrator.run(DB, "migrations", :target => args[:target].to_i)
    Rake::Task['db:version'].execute
  end

  desc "Perform migration reset (full rollback and migration)"
  task :reset do
    Sequel::Migrator.run(DB, "migrations", :target => 0)
    Sequel::Migrator.run(DB, "migrations")
    Rake::Task['db:version'].execute
  end
end

desc "Open a pry session and load model"
task :console do
  verbose(false) do
    sh "pry -I lib/model -r model"
  end
end