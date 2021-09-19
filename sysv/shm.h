//shm.h

#ifndef shm_h
#define shm_h

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define LEN 1024
#define SHM 0x1001
#define SEM_R 0x1002
#define SEM_W 0x1003

//P: 🅿️ in Car Park. Stop, Wait.
//V: VP, UP, Wake UP. they look similar
#define P(sem) semop(sem, &(struct sembuf){.sem_num = 0, .sem_op = -1, .sem_flg = 0}, 1)
#define V(sem) semop(sem, &(struct sembuf){.sem_num = 0, .sem_op = 1, .sem_flg = 0}, 1)

#endif
