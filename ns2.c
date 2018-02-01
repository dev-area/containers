#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static char child_stack[5000000];



int child_fn() {
  int i;
  printf("PID: %ld Parent:%ld\n", (long)getpid(), getppid());
  return 0;
}

int main() {
  pid_t child_pid = clone(child_fn, child_stack+5000, CLONE_NEWPID , NULL);
  printf("clone() = %d\n", child_pid);

  waitpid(child_pid, NULL, 0);
  return 0;
}
