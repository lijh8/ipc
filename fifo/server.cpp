// server.cpp

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <limits.h>

#include "common.h"

int main() {
    mkfifo(FIFO_SERVER_TO_CLIENT, 0666);
    mkfifo(FIFO_CLIENT_TO_SERVER, 0666);
    int fd_server_to_client = open(FIFO_SERVER_TO_CLIENT, O_WRONLY);
    int fd_client_to_server = open(FIFO_CLIENT_TO_SERVER, O_RDONLY);

    signal(SIGPIPE, SIG_IGN);

    char buf[PIPE_BUF]; // write up to PIPE_BUF size is atomic
    unsigned seq = 0;

    for (;;) {
        snprintf(buf, sizeof(buf), "Server: %u", seq++);
        write(fd_server_to_client, buf, sizeof(buf));

        if (read(fd_client_to_server, buf, sizeof(buf)) > 0) {
            printf("%s\n", buf);
        }

    }

    close(fd_server_to_client);
    close(fd_client_to_server);
    unlink(FIFO_SERVER_TO_CLIENT);
    unlink(FIFO_CLIENT_TO_SERVER);
    return 0;
}
