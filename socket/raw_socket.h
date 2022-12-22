#ifndef RAW_SOCKET_H_
#define RAW_SOCKET_H_
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

bool get_mac_address(const char *if_name, struct sockaddr_ll &mac);
int get_interface_id(char *if_name);
bool bind_recv_raw_socket(int sockfd, char *if_name);
bool bind_send_raw_socket(struct sockaddr_ll &dest_addr, char *if_name);

#endif