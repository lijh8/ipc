//shm.h

#ifndef shm_h
#define shm_h

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define LEN 1024
#define SHM 0X1001
#define SEM1 0X1002
#define SEM2 0X1003

#define P(sem, op) do { \
    op.sem_num = 0; \
    op.sem_op = -1; \
    op.sem_flg = 0; \
    semop(sem,&op,1); \
} while (0)

#define V(sem, op) do { \
    op.sem_op = 1; \
    semop(sem,&op,1); \
} while (0)

#endif
