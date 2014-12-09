#include "execute.hpp"
#include "logger.hpp"

extern "C"
int executor(char *root, char *ques, char *path, int sec, int mem) {
  loggerInit(root);
  BOOST_LOG_TRIVIAL(info) << "Root: " << root;
  return execute(root, ques, path, sec, mem);
}
