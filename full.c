#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <string.h>
#define STACK_SIZE (1024 * 1024)

static char stack[STACK_SIZE];

int setip(char *name,char *addr,char *netmask) {
    struct ifreq ifr;
    int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

    strncpy(ifr.ifr_name, name, IFNAMSIZ);
    
    ifr.ifr_addr.sa_family = AF_INET;
    inet_pton(AF_INET, addr, ifr.ifr_addr.sa_data + 2);
    ioctl(fd, SIOCSIFADDR, &ifr);

    inet_pton(AF_INET, netmask, ifr.ifr_addr.sa_data + 2);
    ioctl(fd, SIOCSIFNETMASK, &ifr);

    //get flags
    ioctl(fd, SIOCGIFFLAGS, &ifr);  
    strncpy(ifr.ifr_name, name, IFNAMSIZ);
    ifr.ifr_flags |= (IFF_UP | IFF_RUNNING);
    // set flags
    ioctl(fd, SIOCSIFFLAGS, &ifr);

    return 0;
}

int child(void* arg)
{
  char c;
  sleep(1);
  sethostname("myhost", 6);

  chroot("./fs");
  chdir("/");
  mount("proc", "/proc", "proc", 0, NULL);

  setip("veth1","10.0.0.15","255.0.0.0");
  execlp("/bin/sh", "/bin/sh" , NULL);

  return 1;
}

int main()
{
  char buf[255];
  pid_t pid = clone(child, stack+STACK_SIZE,
      CLONE_NEWNET | CLONE_NEWUTS | CLONE_NEWIPC | CLONE_NEWPID | CLONE_NEWNS | SIGCHLD, NULL);

  sprintf(buf,"sudo ip link add name veth0 type veth peer name veth1 netns %d",pid);
 
  system(buf);
  setip("veth0","10.0.0.13","255.0.0.0");


  waitpid(pid, NULL, 0);
  return 0;
}
