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
directory "run/lock"
directory "bin"
directory "lib/executor"

task default: %w(check build binstubs db:migrate message)

desc "Make necessary directory"
task :fileStruct => %w(run/in run/out run/exe run/ans run/lock bin lib/executor)

task :binstubs do
  sh 'bundle binstubs rake' unless File.exists? "bin/rake"
  sh 'bundle binstubs rspec' unless File.exists? "bin/rspec"
  sh 'bundle binstubs sequel' unless File.exists? "bin/sequel"
end

task :check do
  unless RUBY_VERSION =~ /^2/
    raise "Please use ruby >= 2.0"
  end
end

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

task :message => %w(build db:migrate) do
  puts "Done build"
end

task :server, [:action] do |t, args|
  args.with_defaults(action: "start")
  if args[:action] == "start"
    require_relative 'lib/server'
  elsif args[:action] == "stop"
    Process.kill("INT", File.read("run/lock/server.pid").to_i) if File.exist? "run/lock/server.pid"
    FileUtils.rm "run/lock/server.pid", force: true
  end
end

task :start do
  Rake::Task["server"].invoke("start")
end

task :stop do
  Rake::Task["server"].invoke("stop")
end

task :clean do
  rm_rf "build"
end

namespace :test do
  task :prepare do
    ENV["environment"] = "test"
  end
end

begin
  require 'rspec/core/rake_task'
  RSpec::Core::RakeTask.new(:spec)
  task :spec => %w(test:prepare db:test:reset)
rescue LoadError
end

namespace :db do
  require "sequel"
  Sequel.extension :migration
  DB = Sequel.connect("sqlite://db/judge.db")

  namespace :test do
    file "db/test.db"
    DBTest = Sequel.connect("sqlite://db/test.db")

    task :prepare => "db/test.db" do
      Sequel::Migrator.run(DBTest, "db/migrations")
    end

    task :clear => "db/test.db" do
      Sequel::Migrator.run(DBTest, "db/migrations", target: 0)
    end

    task :reset => %w(db:test:clear db:test:prepare)
  end

  desc "Perform migration up to latest migration available"
  task :migrate do
    Sequel::Migrator.run(DB, "db/migrations")
  end

  desc "Perform rollback to specified target or full rollback as default"
  task :rollback, :target do |t, args|
    args.with_defaults(:target => 0)

    Sequel::Migrator.run(DB, "migrations", :target => args[:target].to_i)
  end

  desc "Perform migration reset (full rollback and migration)"
  task :reset do
    Sequel::Migrator.run(DB, "migrations", :target => 0)
    Sequel::Migrator.run(DB, "migrations")
  end
end

desc "Open a pry session and load model"
task :console do
  verbose(false) do
    sh "pry -I lib/model -r model"
  end
end
