// main.cpp

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <thread>

#define FIFO_MAIN_TO_SUB "/tmp/FIFO_MAIN_TO_SUB"
#define FIFO_SUB_TO_MAIN "/tmp/FIFO_SUB_TO_MAIN"

void sub()
{
    int fd_main_to_sub = open(FIFO_MAIN_TO_SUB, O_RDONLY); // open blocks until opposite O_WRONLY
    int fd_sub_to_main = open(FIFO_SUB_TO_MAIN, O_WRONLY); // open blocks until opposite O_RDONLY

    char buf[PIPE_BUF - 1] = {0}; // write up to PIPE_BUF size is atomic
    int cnt, err;
    unsigned seq = 0;

    for (;;){
        // write
        snprintf(buf, sizeof(buf) - 1, "%u", seq++);
        cnt = write(fd_sub_to_main, buf, sizeof(buf) - 1);
        err = errno;
        if (cnt == -1 && err == EPIPE){
            printf("The read end is closed: %s\n", strerror(err));
            break;
        }

        // read
        int cnt = read(fd_main_to_sub, buf, sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = 0;
        if (cnt == 0) {
            printf("The write end is closed\n");
            break;
        } else if (cnt > 0) {
            printf("%s: %s\n", __func__, buf);
        }
    }

    close(fd_main_to_sub);
    close(fd_sub_to_main);
}

// pipe or fifo is unidirectional.
// it requires two for two-way bidirectional communication.

int main() {
    mkfifo(FIFO_MAIN_TO_SUB, 0666);
    mkfifo(FIFO_SUB_TO_MAIN, 0666);

    std::jthread jt(sub); // start thread first

    int fd_main_to_sub = open(FIFO_MAIN_TO_SUB, O_WRONLY); // open blocks until opposite O_RDONLY
    int fd_sub_to_main = open(FIFO_SUB_TO_MAIN, O_RDONLY); // open blocks until opposite O_WRONLY

    signal(SIGPIPE, SIG_IGN);

    char buf[PIPE_BUF - 1] = {0}; // write up to PIPE_BUF size is atomic
    int cnt, err;
    unsigned seq = 0;

    for (;;) {
        // write
        snprintf(buf, sizeof(buf) - 1, "%u", seq++);
        cnt = write(fd_main_to_sub, buf, sizeof(buf) - 1);
        err = errno;
        if (cnt == -1 && err == EPIPE){
            printf("The read end is closed: %s\n", strerror(err));
            break;
        }

        // read
        cnt = read(fd_sub_to_main, buf, sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = 0;
        if (cnt == 0) {
            printf("The write end is closed\n");
            break;
        } else if (cnt > 0) {
            printf("%s: %s\n", __func__, buf);
        }
    }

    close(fd_main_to_sub);
    close(fd_sub_to_main);
    unlink(FIFO_MAIN_TO_SUB);
    unlink(FIFO_SUB_TO_MAIN);
    return 0;
}
