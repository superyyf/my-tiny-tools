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