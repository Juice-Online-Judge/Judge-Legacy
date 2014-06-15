#ifndef CHILDERRORSTATUS_HPP_INCLUDE
#define CHILDERRORSTATUS_HPP_INCLUDE

enum ChildErrorStatus {
  FileOpenError = 80,
  Dup2Error = 81,
  ExeclpError = 82,
  InFileNotFound = 83
};

const char *errorMessage[] = {
  "Could not open file",
  "dup2 error",
  "exec error",
  "Could not found input file"
};

#define isErrorStatus(x) (x >= 80)
#define getErrorMessage(x) (errorMessage[x - 80])

#endif /* end of include guard: CHILDERRORSTATUS_HPP_INCLUDE */

