/*
 * http://www.adamrisi.com/?p=84
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define __KERNEL__
#include <asm/types.h>
#undef __KERNEL__

#include <linux/ethtool.h>
#include <linux/sockios.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>

/* this is straight from beej's network tutorial. It is a nice wrapper
 * for inet_ntop and helpes to make the program IPv6 ready
 */
char *get_ip_str(const struct sockaddr *sa, char *s, size_t maxlen)
{
  switch(sa->sa_family) {
    case AF_INET:
      inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr), s, maxlen);
      break;

    case AF_INET6:
      inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr), s, maxlen);
      break;

    default:
      strncpy(s, "Unknown AF", maxlen);
      return NULL;
  }

  return s;
}

int main (int argc, char **argv)
{
  char          buf[1024] = {0};
  struct ifconf ifc = {0};
  struct ifreq *ifr = NULL;
  int           sck = 0;
  int           nInterfaces = 0;
  int           i = 0;
  struct ifreq ifr_subnet;
  struct sockaddr *addr_subnet = NULL;

  /* Get a socket handle. */
  sck = socket(AF_INET, SOCK_DGRAM, 0);
  if(-1 == sck) {
    perror("socket");
    return 1;
  }

  /* Query available interfaces. */
  ifc.ifc_len = sizeof(buf);
  ifc.ifc_buf = buf;
  if(-1 == ioctl(sck, SIOCGIFCONF, &ifc)) {
    perror("ioctl(SIOCGIFCONF)");
    close (sck);
    return 1;
  }

  /* Iterate through the list of interfaces. */
  ifr = ifc.ifc_req;
  nInterfaces = ifc.ifc_len / sizeof(struct ifreq);
  for(i = 0; i < nInterfaces; i++)
  {
    struct ifreq *item = &ifr[i];

    /* Show the device name and IP address */
    struct sockaddr *addr = &(item->ifr_addr);
    char ip[INET6_ADDRSTRLEN];
    printf("%s: IP %s",
           item->ifr_name,
           get_ip_str(addr, ip, INET6_ADDRSTRLEN));

    /* Get the MAC address */
    if(-1 == ioctl(sck, SIOCGIFHWADDR, item)) {
      perror("ioctl(SIOCGIFHWADDR)");
      close (sck);
      return 1;
    }

    /* display result */
    printf(", MAC %.2x:%.2x:%.2x:%.2x:%.2x:%.2x ",
           (unsigned char)item->ifr_hwaddr.sa_data[0],
           (unsigned char)item->ifr_hwaddr.sa_data[1],
           (unsigned char)item->ifr_hwaddr.sa_data[2],
           (unsigned char)item->ifr_hwaddr.sa_data[3],
           (unsigned char)item->ifr_hwaddr.sa_data[4],
           (unsigned char)item->ifr_hwaddr.sa_data[5]);

    /* Reset the memory allocated for ifreq */
    memset (&ifr_subnet, 0, sizeof (struct ifreq));

    /* Update interface name into ifreq */
    strcpy (ifr_subnet.ifr_name, item->ifr_name);

    /* Call ioctl function */
    if(-1 == ioctl (sck, SIOCGIFNETMASK, &ifr_subnet ))
    {
      perror("ioctl(SIOCGIFNETMASK)");
      close (sck);
      return 1;
    }
    addr_subnet = &(ifr_subnet.ifr_addr);
    printf("SubnetMask: %s\n", get_ip_str(addr_subnet, ip, INET6_ADDRSTRLEN));
  }

  close (sck);
  return 0;
}
