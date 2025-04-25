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
    char buf[PIPE_BUF]; // Writes of up to PIPE_BUF bytes are guaranteed to be atomic

    // Create the FIFO if it doesn't exist
    mkfifo(FIFO_SERVER_TO_CLIENT, 0666);

    printf("Writer: Opening FIFO...\n");
    fd = open(FIFO_SERVER_TO_CLIENT, O_WRONLY);
    printf("Writer: FIFO opened\n");

    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR){
        perror("signal");
        return -1;
    }

    while (1) {
        printf("Enter message (type 'exit' to quit): ");
        fgets(buf, sizeof(buf), stdin);

        // Remove newline if present
        buf[strcspn(buf, "\n")] = 0;

        // Write to FIFO
        write(fd, buf, strlen(buf)+1); // +1 to include null terminator

        if (strcmp(buf, "exit") == 0) {
            break;
        }
    }

    close(fd);
    unlink(FIFO_SERVER_TO_CLIENT); // Remove the FIFO
    return 0;
}
