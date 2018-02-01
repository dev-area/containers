#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

static char child_stack[5000];


void grchild(int num)
{
  printf("child(%d) in ns my PID: %d Parent ID=%d\n", num, getpid(),getppid());
  sleep(5);
  puts("end child");
}

int child_fn(int ppid) {
  int i;
  printf("PID: %ld Parent:%ld\n", (long)getpid(), getppid());
  for(i=0;i<3;i++)
  {
   	if(fork() == 0)
  	{
  		grchild(i+1);  
  		exit(0);
  	}
  	kill(ppid,SIGKILL); // no effect 
  }
  sleep(2);
  kill(2,SIGKILL); // kill the first child
  sleep(10);
  return 0;
}

int main() {
  pid_t pid = clone(child_fn, child_stack+5000, CLONE_NEWPID , getpid());
  printf("clone() = %d\n", pid);

  waitpid(pid, NULL, 0);
  return 0;
}
