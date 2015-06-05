#include "child_error_status.hpp"

const char *error_message[] = {
  "Could not open file",
  "dup2 error",
  "exec error",
  "Could not found input file"
};

bool is_error_status(int x) {
  return x >= 80;
}
const char *get_error_message(int x) {
  return error_message[x - 80];
}
