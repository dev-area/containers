#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <string.h>

int main(int argc, const char *argv[]) {
    struct ifreq ifr;
    const char * name = "ens33";
    int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

    strncpy(ifr.ifr_name, name, IFNAMSIZ);

 //   struct sockaddr_in* addr = (struct sockaddr_in*)&ifr.ifr_addr;
 //   inet_pton(AF_INET, "10.12.0.1", &addr->sin_addr);
    
    ifr.ifr_addr.sa_family = AF_INET;
    inet_pton(AF_INET, "10.12.0.1", ifr.ifr_addr.sa_data + 2);
    ioctl(fd, SIOCSIFADDR, &ifr);

    inet_pton(AF_INET, "255.255.0.0", ifr.ifr_addr.sa_data + 2);
    ioctl(fd, SIOCSIFNETMASK, &ifr);

    ioctl(fd, SIOCGIFFLAGS, &ifr);
    strncpy(ifr.ifr_name, name, IFNAMSIZ);
    ifr.ifr_flags |= (IFF_UP | IFF_RUNNING);

    ioctl(fd, SIOCSIFFLAGS, &ifr);

    return 0;
}
