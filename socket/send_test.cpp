#include "raw_socket.h"

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

  struct sockaddr_ll dest_addr;
  // if you use sudo, please change argv[1] to argv[2]
  if (!bind_send_raw_socket(dest_addr, argv[1])) {
    exit(EXIT_FAILURE);
  }

  uint8_t buff[MSG_LENGTH] = {0xAA, 0xBB, 0xCC, 0xDD};
  while (true) {
    int ret = sendto(fd, buff, sizeof(buff), 0, (struct sockaddr *)&dest_addr,
                     sizeof(dest_addr));
    if (ret == -1) {
      perror("send failed");
    }
    sleep(1);
  }
}