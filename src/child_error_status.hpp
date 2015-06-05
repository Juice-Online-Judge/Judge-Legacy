#ifndef CHILDERRORSTATUS_HPP_INCLUDE
#define CHILDERRORSTATUS_HPP_INCLUDE

enum ChildErrorStatus {
  FILE_OPEN_ERROR = 80,
  DUP2_ERROR = 81,
  EXEC_ERROR = 82,
  INPUT_FILE_NOT_FOUND_ERROR = 83
};

bool is_error_status(int);
const char *get_error_message(int);

#endif /* end of include guard: CHILDERRORSTATUS_HPP_INCLUDE */

