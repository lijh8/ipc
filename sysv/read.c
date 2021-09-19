//read.c
//ipcs
//ipcrm sem 12
//ipcrm shm 34
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include "shm.h"

void *g_shmptr = NULL;
int g_shmid;
int g_sem_r;
int g_sem_w;
struct sembuf g_semop;

void handler(int sig)
{
    //don't remove it, just leave it there
    //so, read, write, each side can quit or start at any time
    // shmdt(g_shmptr);
    // semctl(g_sem_r,0,IPC_RMID);
    // semctl(g_sem_w,0,IPC_RMID);
    // exit(0);
}

void init()
{
    // signal(SIGINT,handler); //don't remove it, just leave it there

    g_shmid = shmget(SHM,LEN,IPC_CREAT|0666);
    if(g_shmid==-1){
        perror("shmget");
        exit(-1);
    }

    g_shmptr = shmat(g_shmid,0,0);
    if(g_shmptr==(void *)-1){
        perror("shmat");
        exit(-1);
    }

    g_sem_r = semget(SEM1,1,IPC_CREAT|0666);
    if(g_sem_r==-1){
        perror("semget");
        exit(-1);
    }

    g_sem_w = semget(SEM2,1,IPC_CREAT|0666);
    if(g_sem_w==-1){
        perror("semget");
        exit(-1);
    }
}

int main()
{
    init();

    while(1){
        printf("%s: %s\n", __FILE__, "P");
        P(g_sem_w, g_semop);

        // printf("%s: %s\n", __FILE__, (char *)g_shmptr);

        V(g_sem_r, g_semop);
        printf("%s: %s\n", __FILE__, "V");
    }

    return 0;
}
