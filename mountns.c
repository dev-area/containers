#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>

#define STACK_SIZE (1024 * 1024)

// run with sudo
// use ps , hostname, etc.

static char stack[STACK_SIZE];

int child(void* arg)
{
  char c;

  sethostname("myhost", 6);

  chroot("./fs");
  chdir("/");
  mount("proc", "/proc", "proc", 0, NULL);

  execlp("/bin/sh", "/bin/sh" , NULL);
  printf("Ooops\n");
  return 1;
}

int main()
{
  pid_t pid = clone(child, stack+STACK_SIZE,
      CLONE_NEWUTS | CLONE_NEWIPC | CLONE_NEWPID | CLONE_NEWNS | SIGCHLD, NULL);


  waitpid(pid, NULL, 0);
  return 0;
}
