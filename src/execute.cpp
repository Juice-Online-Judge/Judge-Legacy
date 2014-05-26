#include <iostream>
#include <string>
#include <cstdio>

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/syscall.h>
#include <sys/reg.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>

#include <boost/filesystem.hpp>

#include "judgeStatus.hpp"
#include "execute.hpp"

using namespace std;

pid_t child;
volatile int childStart = 0;

static void startTrace(int);
static void setupRLimit(int, rlim_t);

int execute(string quesName, string pathStr, int timeLimit, int memoryLimit) {
  int res = PASS;
  if(signal(SIGUSR1, startTrace) == SIG_ERR) {
    cerr << "Error: Unable to create signal handler for SIGUSR1" << endl;
  }
  child = fork();
  if(child == 0) {
    boost::filesystem::path pwd(boost::filesystem::current_path()), exePath(pathStr);
    string inFileName((pwd.parent_path() / "run" / "input" / (quesName + ".in")).string());
    string outFileName((pwd.parent_path() / "run" / "ans" / (exePath.filename().string() + ".ans")).string());
    freopen(inFileName.c_str(), "r", stdin);
    freopen(outFileName.c_str(), "w", stdout);
    setupRLimit(RLIMIT_NPROC, 1);
    setupRLimit(RLIMIT_NOFILE, 2);
    setupRLimit(RLIMIT_MEMLOCK, 0);
    setupRLimit(RLIMIT_AS, memoryLimit * 1024);
    kill(getppid(), SIGUSR1);
    execlp(pathStr.c_str(), pathStr.c_str());
  }
  if(child < 0) {
    return RE;
  }

  while(1) {
    int status;
    int syscall;
    wait(&status);
    if(WIFEXITED(status)) {
      break;
    }
    if(WIFSIGNALED(status)) {
      res = RE;
      break;
    }
    if(childStart) {
      syscall = ptrace(PTRACE_PEEKUSER, child, 8 * ORIG_RAX, NULL);
      if(syscall == __NR_fork) {
        ptrace(PTRACE_SYSCALL, child, NULL, NULL);
      }
      else {
        ptrace(PTRACE_SYSCALL, child, NULL, NULL);
      }
    }
  }
  return res;
}

static void startTrace(int signo) {
  childStart = 1;
}

static void setupRLimit(int res, rlim_t limit) {
  rlimit rl = { .rlim_cur = limit, .rlim_max = limit };
  setrlimit(res, &rl);
}
