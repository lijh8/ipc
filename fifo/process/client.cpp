// client.cpp

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

#include "common.h"

int main() {
    mkfifo(FIFO_SERVER_TO_CLIENT, 0666);
    mkfifo(FIFO_CLIENT_TO_SERVER, 0666);
    int fd_server_to_client = open(FIFO_SERVER_TO_CLIENT, O_RDONLY); // open blocks until opposite O_WRONLY
    int fd_client_to_server = open(FIFO_CLIENT_TO_SERVER, O_WRONLY); // open blocks until opposite O_RDONLY

    signal(SIGPIPE, SIG_IGN);

    char buf[PIPE_BUF - 1] = {0}; // write up to PIPE_BUF size is atomic
    unsigned seq = 0;

    for (;;) {
        snprintf(buf, sizeof(buf) - 1, "%u", seq++);
        int cnt = write(fd_client_to_server, buf, sizeof(buf) - 1);
        int err = errno;
        if (cnt == -1 && err == EPIPE){
            printf("The read end is closed: %s\n", strerror(err));
            break;
        }

        cnt = read(fd_server_to_client, buf, sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = 0;
        if (cnt == 0) {
            printf("The write end is closed\n");
            break;
        } else if (cnt > 0) {
            printf("from server: %s\n", buf);
        }

        // sleep(1); // test
    }

    close(fd_server_to_client);
    close(fd_client_to_server);
    unlink(FIFO_SERVER_TO_CLIENT);
    unlink(FIFO_CLIENT_TO_SERVER);
    return 0;
}
