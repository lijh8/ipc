//write.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "shm.h"

void *g_shmptr = NULL;
int g_sem_r;
int g_sem_w;

//don't remove sem, shm in the code, just leave them there.
//so read, write, each side can quit or start at any time.
//use ipcrm to remove them from command line.
void init()
{
    union semun arg;

    int shmid = shmget(SHM, LEN, IPC_CREAT | 0666);
    if(shmid == -1){
        perror("shmget");
        exit(-1);
    }

    g_shmptr = shmat(shmid, 0, 0);
    if(g_shmptr == (void *)-1){
        perror("shmat");
        exit(-1);
    }

    g_sem_r = semget(SEM_R, 1, IPC_CREAT | 0666);
    if(g_sem_r == -1){
        perror("semget");
        exit(-1);
    }

    g_sem_w = semget(SEM_W, 1, IPC_CREAT | 0666);
    if(g_sem_w == -1){
        perror("semget");
        exit(-1);
    }

    arg.val = 1;
    int rt = semctl(g_sem_r, 0, SETVAL, arg);  //set 1
    if(rt == -1){
        perror("semctl");
        exit(-1);
    }

    arg.val = 0;
    rt = semctl(g_sem_w, 0, SETVAL, arg);
    if(rt == -1){
        perror("semctl");
        exit(-1);
    }
}

int main()
{
    init();

    while(1){
        printf("%s: %s\n", __FILE__, "P");
        P(g_sem_r);

        strncpy((char*)g_shmptr, "hello", LEN - 1); // ok
        // memcpy(g_shmptr, "hello", LEN - 1); // err: -Wstringop-overread
        ((char*)g_shmptr)[LEN-1] = 0;
        sleep(1); //

        V(g_sem_w);
        printf("%s: %s\n", __FILE__, "V");
    }

    return 0;
}
