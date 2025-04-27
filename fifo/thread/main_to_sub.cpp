// main_to_sub.cpp

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <thread>

#define MAIN_TO_SUB "/tmp/MAIN_TO_SUB"

void sub()
{
    int fd = open(MAIN_TO_SUB, O_RDONLY); // open blocks until opposite O_WRONLY

    char buf[PIPE_BUF - 1] = {0}; // write up to PIPE_BUF size is atomic

    for (;;){
        int cnt = read(fd, buf, sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = 0;
        if (cnt == 0) {
            printf("All peer offline\n");
            break;
        } else if (cnt > 0) {
            printf("seq: %s\n", buf);
        }
    }

    close(fd);
}

// main -> sub
int main() {
    mkfifo(MAIN_TO_SUB, 0666);
    std::jthread jt(sub);
    int fd = open(MAIN_TO_SUB, O_WRONLY); // open blocks until opposite O_RDONLY
    signal(SIGPIPE, SIG_IGN);
    char buf[PIPE_BUF - 1] = {0}; // write up to PIPE_BUF size is atomic

    unsigned seq = 0;
    for (;;) {
        snprintf(buf, sizeof(buf) - 1, "%u", seq++);
        int cnt = write(fd, buf, sizeof(buf) - 1);
        int err = errno;
        if (cnt == -1 && err == EPIPE){
            printf("%s\n", strerror(err));
            break;
        }
    }

    close(fd);
    unlink(MAIN_TO_SUB);
    return 0;
}
