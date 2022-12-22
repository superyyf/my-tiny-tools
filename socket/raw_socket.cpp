#include <arpa/inet.h>
#include <errno.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#define MSG_LENGTH 64

bool get_mac_address(const char *if_name, struct sockaddr_ll &mac) {
  struct ifreq ifr;

  // create socket fd for ioctl operation
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
    perror("socket fd create failed");
    return false;
  }

  memset(&ifr, 0, sizeof(ifr));
  strcpy(ifr.ifr_name, if_name);
  if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0) {
    perror("get mac address failed");
    return false;
  }

  memcpy(mac.sll_addr, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
  close(fd);
  return true;
}

bool bind_recv_raw_socket(int sockfd, char *if_name) {
  struct sockaddr_ll mac_addr;
  if (!get_mac_address(if_name, mac_addr)) {
    return false;
  }

  mac_addr.sll_family = AF_PACKET;

  // bind recv raw_socket to specific interface using interface name
  if (bind(sockfd, (struct sockaddr *)&mac_addr, sizeof(mac_addr)) == -1) {
    perror("bind recv interface failed");
    return false;
  }
  return true;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("argument error\n");
    exit(EXIT_FAILURE);
  }

  int fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (fd == -1) {
    perror("socket init failed");
    exit(EXIT_FAILURE);
  }

  // if you use sudo, please change argv[1] to argv[2]
  if (!bind_recv_raw_socket(fd, argv[1])) {
    exit(EXIT_FAILURE);
  }

  uint8_t buff[MSG_LENGTH] = {};
  while (true) {
    int ret = recvfrom(fd, buff, MSG_LENGTH, 0, NULL, NULL);
    if (ret == -1) {
      perror("recvfrom failed");
    } else {
      for (int i = 0; i < MSG_LENGTH; ++i) {
        printf("%02X ", buff[i]);
      }
      printf("\n");
    }
    sleep(1);
  }
}