//write.c
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

#define LEN 1024
#define SHMNAME "/SHMNAME"
#define SEM1 "SEM1"
#define SEM2 "SEM2"
sem_t *sem1;
sem_t *sem2;
char *addr;

void handler(int sig){
    sem_close(sem1);
    sem_close(sem2);
    sem_unlink(SEM1);
    sem_unlink(SEM2);
    munmap(addr, LEN);
    shm_unlink(SHMNAME);
    exit(0);
}

int main(int argc, char *argv[])
{
    int fd = shm_open(SHMNAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1){
        perror("shm_open");
        return -1;
    }
    if (ftruncate(fd, LEN) == -1){
        perror("ftruncate");
        return -1;
    }

    addr = mmap(NULL, LEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED){
        perror("mmap");
        return -1;
    }

    sem1 = sem_open("sem1", O_CREAT, 0666, 1);
    if(sem1 == SEM_FAILED){
        perror("sem_open");
        return -1;
    }

    sem2 = sem_open("sem2", O_CREAT, 0666, 0);
    if(sem2 == SEM_FAILED){
        perror("sem_open");
        return -1;
    }

    // signal(SIGINT, handler);
    // signal(SIGTERM, handler);

    while (fgets(addr, LEN, stdin)){
        sem_wait(sem1);
        //...
        sem_post(sem2);
    }

    exit(0);
}
