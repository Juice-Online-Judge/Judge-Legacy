#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>

#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/syscall.h>
#include <sys/reg.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/user.h>

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include "judgeStatus.hpp"
#include "childErrorStatus.hpp"
#include "execute.hpp"
#include "logger.hpp"

using namespace std;

namespace {
  pid_t child;

  volatile int childStart = 0;
  volatile int timeLimit;
  volatile int res = PASS;

  void startTrace(int);
  void timeLimitExceed(int);
  void setupRLimit(int, rlim_t);
}

int execute(const string& projectRoot, const string& quesName, const string& pathStr, int timeLimit, int memoryLimit) {
  FILE *fp;
  user_regs_struct uregs;
  BOOST_LOG_TRIVIAL(info) << "Judge: " << pathStr;
  if(signal(SIGUSR1, startTrace) == SIG_ERR) {
    BOOST_LOG_TRIVIAL(error) << "Error: Unable to create signal handler for SIGUSR1";
    return RE;
  }
  if(signal(SIGALRM, timeLimitExceed) == SIG_ERR) {
    BOOST_LOG_TRIVIAL(error) << "Error: Unable to create signal handler for SIGALRM";
    return RE;
  }
  ::timeLimit = timeLimit;
  child = fork();
  if(child == 0) {
    boost::filesystem::path pwd(projectRoot), exePath(pathStr);
    string inFileName((pwd / "run" / "in" / (quesName + ".in")).string());
    string outFileName((pwd / "run" / "ans" / (exePath.filename().string() + ".ans")).string());
    BOOST_LOG_TRIVIAL(info) << "Input file: " << inFileName;
    BOOST_LOG_TRIVIAL(info) << "Output file: " << outFileName;
    if(!boost::filesystem::exists(boost::filesystem::path(inFileName)))
      exit(InFileNotFound);
    if(boost::filesystem::exists(boost::filesystem::path(outFileName))) {
      BOOST_LOG_TRIVIAL(info) << "Output file exist, remove it";
      boost::filesystem::remove(boost::filesystem::path(outFileName));
    }
    DIR *dirp;
    struct dirent *entry;
    int dfd;
    fp = fopen(outFileName.c_str(), "w");
    dup2(fileno(fp), 1);
		int fd = open(inFileName.c_str(), O_RDONLY, 0644);
    int outFd = open(outFileName.c_str(), O_WRONLY);
		if(fd < 0)
      exit(FileOpenError);
    if(outFd < 0)
      exit(FileOpenError);
		if(dup2(fd, STDIN_FILENO) < 0)
      exit(Dup2Error);
		if(dup2(outFd, STDOUT_FILENO) < 0)
      exit(Dup2Error);
    close(fd);
    close(outFd);
    dirp = opendir("/proc/self/fd");
    dfd = dirfd(dirp);
    while((entry = readdir(dirp))) {
      int fd;
      if(not strcmp(entry->d_name, ".") or not strcmp(entry->d_name, "..")) {
        continue;
      }
      fd = boost::lexical_cast<int>(entry->d_name);
      if(fd > 2 and fd != dfd) {
        close(fd);
      }
    }
    closedir(dirp);
    setupRLimit(RLIMIT_NPROC, 1);
    setupRLimit(RLIMIT_NOFILE, 64);
    setupRLimit(RLIMIT_MEMLOCK, 0);
    setupRLimit(RLIMIT_AS, (memoryLimit + 15) * 1024 * 1024);
    kill(getppid(), SIGUSR1);
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    execlp(pathStr.c_str(), pathStr.c_str(), NULL);
    exit(ExeclpError);
  }
  if(child < 0) {
    return RE;
  }

  while(1) {
    int status;
    int syscall;
    wait(&status);
    if(WIFEXITED(status)) {
      alarm(0); // cancel
      BOOST_LOG_TRIVIAL(info) << "Child process exit with status: " << WEXITSTATUS(status);
      if(isErrorStatus(WEXITSTATUS(status))) {
        BOOST_LOG_TRIVIAL(error) << "Child occur error: " << getErrorMessage(WEXITSTATUS(status));
        res = RE;
      }
      break;
    }
    if(WIFSIGNALED(status)) {
      alarm(0);  //cancel
      if(res == PASS)
        res = RE;
      BOOST_LOG_TRIVIAL(info) << "Child process got signal: " << WTERMSIG(status) << endl;
      break;
    }
    ptrace(PTRACE_GETREGS, child, 0, &uregs);
#ifdef __x86_64__
    syscall = uregs.orig_rax;
#else
    syscall = uregs.orig_eax;
#endif
    BOOST_LOG_TRIVIAL(info) << "Child call syscall: " << syscall;
    if((syscall == SYS_fork || syscall == SYS_clone) && childStart) {
      BOOST_LOG_TRIVIAL(info) << "Child call fork, kill";
      ptrace(PTRACE_KILL, child, NULL, NULL);
    }
    else {
      ptrace(PTRACE_SYSCALL, child, NULL, NULL);
    }
  }
  return res;
}

namespace {
  void startTrace(int /*signo*/) {
    BOOST_LOG_TRIVIAL(info) << "Start Trace";
    alarm(timeLimit);
    ptrace(PTRACE_SYSCALL, child, NULL, NULL);
    childStart = 1;
  }

  void timeLimitExceed(int /*signo*/) {
    static int occurTime = 0;
    BOOST_LOG_TRIVIAL(info) << "Time exceed";
    if(occurTime) {
      BOOST_LOG_TRIVIAL(info) << "Tle kill";
      kill(child, SIGKILL);  // time limit exceed twice. kill it
    }
    else {
      res = TLE;
      occurTime++;
      alarm(1); // give more time
    }
  }

  void setupRLimit(int res, rlim_t limit) {
    rlimit rl = { .rlim_cur = limit, .rlim_max = limit };
    setrlimit(res, &rl);
  }
}
