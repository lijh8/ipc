#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <signal.h>

#include "common.h"

int main() {
    int fd;
    char buf[PIPE_BUF];

    printf("Reader: Opening FIFO...\n");
    fd = open(FIFO_SERVER_TO_CLIENT, O_RDONLY);
    printf("Reader: FIFO opened\n");

    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR){
        perror("signal");
        return -1;
    }

    while (1) {
        // Read from FIFO
        int bytes_read = read(fd, buf, sizeof(buf));
        if (bytes_read <= 0) {
            perror("read");
            break;
        }

        printf("Received: %s\n", buf);

        if (strcmp(buf, "exit") == 0) {
            break;
        }
    }

    close(fd);
    return 0;
}
