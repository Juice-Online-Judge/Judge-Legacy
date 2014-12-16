#include "logger.hpp"

#include <iostream>
#include <fstream>
#include <memory>
#include <mutex>
#include <string>
#include <sstream>
#include <utility>

#include <boost/filesystem.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/utility/empty_deleter.hpp>
#include <boost/shared_ptr.hpp>

using namespace std;

namespace {
  mutex instanceMutex;
  string path;
};

Logger *Logger::logger = nullptr;

Logger::Logger(const string& path) {
  using namespace boost::log;
  boost::filesystem::path projectRoot(boost::filesystem::absolute(boost::filesystem::path(path)));
  auto logPath = projectRoot / "run" / "executor.log";
  boost::shared_ptr<sinks::text_ostream_backend> backend(new sinks::text_ostream_backend());
  backend->add_stream(boost::shared_ptr<ostream>(&clog, boost::empty_deleter()));
  backend->add_stream(boost::shared_ptr<ofstream>(new ofstream(logPath.string(), ofstream::app)));
  backend->auto_flush(true);
  typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_ostream_backend > sink_t;
  boost::shared_ptr< sink_t > sink(new sink_t(backend));
  // sink format
  sink->set_formatter(
    boost::log::expressions::stream
      << boost::log::expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %T")
      << "[" << boost::log::trivial::severity  << "]\t"
      << boost::log::expressions::smessage
  );
  // sink filter
  sink->set_filter(
    boost::log::trivial::severity > boost::log::trivial::debug
  );
  // add sink
  boost::shared_ptr< boost::log::core > core = boost::log::core::get();
  core->add_sink(sink);
  // setup common attributes
  boost::log::add_common_attributes();

  BOOST_LOG_TRIVIAL(info) << "Logger start";
}

void Logger::Init(const char *path) {
  ::path = path;
}

Logger& Logger::GetInstance() {
  static Logger *tmp;
  if(logger == nullptr) {
    lock_guard<mutex> mLock(instanceMutex);
    if(logger == nullptr) {
      tmp = new Logger(::path);
      if(logger == nullptr) {
        logger = tmp;
      }
    }
  }
  return *logger;
}

Logger::LoggerWrapper Logger::info() {
  LoggerWrapper wrapper(LoggerWrapper::i);
  return wrapper;
}

Logger::LoggerWrapper Logger::debug() {
  LoggerWrapper wrapper(LoggerWrapper::d);
  return wrapper;
}

Logger::LoggerWrapper Logger::error() {
  LoggerWrapper wrapper(LoggerWrapper::e);
  return wrapper;
}

Logger::LoggerWrapper::~LoggerWrapper() {
  switch(l) {
    case i:
      BOOST_LOG_TRIVIAL(info) << sstr->str();
      break;
    case d:
      BOOST_LOG_TRIVIAL(debug) << sstr->str();
      break;
    case e:
      BOOST_LOG_TRIVIAL(error) << sstr->str();
      break;
  }
}

Logger::LoggerWrapper& Logger::LoggerWrapper::operator<<(int data) {
  (*sstr) << data;
  return *this;
}

