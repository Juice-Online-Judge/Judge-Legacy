#ifndef LOGGER_HPP_INCLUDE
#define LOGGER_HPP_INCLUDE

#include <sstream>
#include <memory>
#include <string>
#include <utility>

class Logger {
private:
  struct LoggerWrapper {
    enum level{d, i, e};
    LoggerWrapper(level l) : l(l), sstr(new std::stringstream){};
    LoggerWrapper(LoggerWrapper&& wrapper) : l(wrapper.l), sstr(std::move(wrapper.sstr)){};
    ~LoggerWrapper();
    template <typename T>
    LoggerWrapper& operator<<(T& data);
    LoggerWrapper& operator<<(int);
  private:
    level l;
    std::unique_ptr<std::stringstream> sstr;
  };
public:
  static void Init(const char *);
  static Logger& GetInstance();
  LoggerWrapper info();
  LoggerWrapper debug();
  LoggerWrapper error();
private:
  Logger(const std::string&);
  Logger(Logger&) = delete;
  static Logger *logger;
  void operator=(Logger&) = delete;
};

#include "logger_priv.hpp"

#endif /* end of include guard: LOGGER_HPP_INCLUDE */

