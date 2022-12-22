#include "raw_socket.h"

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

int get_interface_id(char *if_name) {
  struct ifreq ifr;

  // create socket fd for ioctl operation
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
    perror("socket fd create failed");
    return -1;
  }

  memset(&ifr, 0, sizeof(ifr));
  strcpy(ifr.ifr_name, if_name);
  if (ioctl(fd, SIOCGIFINDEX, &ifr) < 0) {
    perror("get interface index failed");
    return -1;
  }

  close(fd);
  return ifr.ifr_ifindex;
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

bool bind_send_raw_socket(struct sockaddr_ll &dest_addr, char *if_name) {
  int if_idx = get_interface_id(if_name);
  if (if_idx == -1) {
    return false;
  }
  dest_addr.sll_ifindex = if_idx;
  return true;
}
