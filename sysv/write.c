//write.c
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
    shmdt(g_shmptr);
    shmctl(g_shmid,IPC_RMID,0);
    exit(0);
}

void init()
{
    // signal(SIGINT,handler);

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

    int res = semctl(g_sem_r,0,SETVAL,1);
    if(res==-1){
        perror("semctl");
        exit(-1);
    }

    res = semctl(g_sem_w,0,SETVAL,0);
    if(res==-1){
        perror("semctl");
        exit(-1);
    }
}

int main()
{
    init();

    while(1){
        printf("%s: %s\n", __FILE__, "P");
        P(g_sem_r, g_semop);

        char *p = (char*)g_shmptr;
        memcpy(p, "hello", LEN - 1);
        p[LEN-1] = 0;
        sleep(1);//

        V(g_sem_w, g_semop);
        printf("%s: %s\n", __FILE__, "V");
    }

    return 0;
}
