#include "execute.hpp"
#include "logger.hpp"

#include <string>

using namespace std;

namespace {
  string root;
};

extern "C" {
  void executorInit(char *root) {
    Logger::Init(root);
    ::root = root;
    Logger& logger = Logger::GetInstance();
    logger.info() << "Executor Initialize";
    logger.info() << "Root: " << root;
  }

  int executor(char *ques, char *path, int sec, int mem) {
    return execute(root, ques, path, sec, mem);
  }
}
