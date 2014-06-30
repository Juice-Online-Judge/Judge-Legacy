#include "execute.hpp"

extern "C"
int executor(char *ques, char *path, int sec, int mem) {
  return execute(ques, path, sec, mem);
}
