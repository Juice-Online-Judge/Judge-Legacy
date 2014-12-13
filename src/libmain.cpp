#include "execute.hpp"
#include "logger.hpp"

#include <string>

using namespace std;

namespace {
  string root;
};

extern "C" {
  void executorInit(char *root) {
    ::root = root;
    loggerInit(root);
    BOOST_LOG_TRIVIAL(info) << "Executor Initialize";
    BOOST_LOG_TRIVIAL(info) << "Root: " << root;
  }

  int executor(char *ques, char *path, int sec, int mem) {
    return execute(root, ques, path, sec, mem);
  }
}
