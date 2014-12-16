#define BOOST_LOG_DYN_LINK
#include <boost/log/trivial.hpp>

template<typename T>
Logger::LoggerWrapper& Logger::LoggerWrapper::operator<<(T& data) {
  (*sstr) << data;
  return *this;
}
