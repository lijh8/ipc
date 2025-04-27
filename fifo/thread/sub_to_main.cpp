// sub_to_main.cpp

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <thread>

#define SUB_TO_MAIN "/tmp/SUB_TO_MAIN"

void sub()
{
    int fd = open(SUB_TO_MAIN, O_WRONLY); // open blocks until opposite O_RDONLY

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
}

// sub -> main
int main() {
    mkfifo(SUB_TO_MAIN, 0666);
    std::jthread jt(sub);
    int fd = open(SUB_TO_MAIN, O_RDONLY); // open blocks until opposite O_WRONLY
    signal(SIGPIPE, SIG_IGN);
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
    unlink(SUB_TO_MAIN);
    return 0;
}
