#include <iostream>
#include <string>
#include <cstdio>

#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/syscall.h>
#include <sys/reg.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/user.h>

#include <boost/filesystem.hpp>

#include "judgeStatus.hpp"
#include "execute.hpp"

using namespace std;

pid_t child;
volatile int childStart = 0;

static void startTrace(int);
static void setupRLimit(int, rlim_t);

int execute(const string& quesName, const string& pathStr, int timeLimit, int memoryLimit) {
  int res = PASS;
  FILE *fp;
  user_regs_struct uregs;
  if(signal(SIGUSR1, startTrace) == SIG_ERR) {
    cerr << "Error: Unable to create signal handler for SIGUSR1" << endl;
  }
  child = fork();
  if(child == 0) {
    boost::filesystem::path pwd(boost::filesystem::current_path()), exePath(pathStr);
    string inFileName((pwd.parent_path() / "run" / "in" / (quesName + ".in")).string());
    string outFileName((pwd.parent_path() / "run" / "ans" / (exePath.filename().string() + ".ans")).string());
    fp = fopen(outFileName.c_str(), "w");
    dup2(fileno(fp), 1);
		int fd = open(inFileName.c_str(), O_RDONLY, 0644);
    int outFd = open(outFileName.c_str(), O_WRONLY);
		if(fd < 0)
      exit(89);
    if(outFd < 0)
      exit(90);
		if(dup2(fd, STDIN_FILENO) < 0)
      exit(91);
		if(dup2(outFd, STDOUT_FILENO) < 0)
      exit(92);
    close(fd);
    close(outFd);
    setupRLimit(RLIMIT_NPROC, 1);
    setupRLimit(RLIMIT_NOFILE, 64);
    setupRLimit(RLIMIT_MEMLOCK, 0);
    setupRLimit(RLIMIT_AS, (memoryLimit + 15) * 1024 * 1024);
    kill(getppid(), SIGUSR1);
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    execlp(pathStr.c_str(), pathStr.c_str(), NULL);
  }
  if(child < 0) {
    return RE;
  }

  while(1) {
    int status;
    int syscall;
    wait(&status);
    if(WIFEXITED(status)) {
      cout << "child exit:" << WEXITSTATUS(status) << endl;
      break;
    }
    if(WIFSIGNALED(status)) {
      res = RE;
      cout << "child got signal " << WTERMSIG(status) << endl;
      break;
    }
    ptrace(PTRACE_GETREGS, child, 0, &uregs);
    syscall = uregs.orig_rax;
    cout << "child call: " << syscall  << endl;
    if((syscall == SYS_fork || syscall == SYS_clone) && childStart) {
      cout << "child call fork" << endl;
      ptrace(PTRACE_KILL, child, NULL, NULL);
    }
    else {
      ptrace(PTRACE_CONT, child, NULL, NULL);
      ptrace(PTRACE_SYSCALL, child, NULL, NULL);
    }
  }
  return res;
}

static void startTrace(int signo) {
  cout << "start judge" << endl;
  ptrace(PTRACE_SYSCALL, child, NULL, NULL);
  childStart = 1;
}

static void setupRLimit(int res, rlim_t limit) {
  rlimit rl = { .rlim_cur = limit, .rlim_max = limit };
  setrlimit(res, &rl);
}
