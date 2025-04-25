//shm.h
#ifndef shm_h
#define shm_h

#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define LEN 1024
#define SHM 0x1001
#define SEM_R 0x1002
#define SEM_W 0x1003

//P: 🅿️ in Car Park. Stop, Wait.
//V: VP, UP, Wake UP. they look similar

// error: Taking the address of a temporary object of type 'T'

// #define P(sem) semop(sem, &(struct sembuf){.sem_num = 0, .sem_op = -1, .sem_flg = 0}, 1)
// #define V(sem) semop(sem, &(struct sembuf){.sem_num = 0, .sem_op = 1, .sem_flg = 0}, 1)

#define P(semid) do { \
    struct sembuf sops = {.sem_num = 0, .sem_op = -1, .sem_flg = 0}; \
    errno = 0; \
    auto n = semop(semid, &sops, 1); \
    if (n != 0){ \
        perror("P semop"); \
    } \
} while (0)

#define V(semid) do { \
    struct sembuf sops = {.sem_num = 0, .sem_op = 1, .sem_flg = 0}; \
    errno = 0; \
    auto n = semop(semid, &sops, 1); \
    if (n != 0){ \
       perror("V semop"); \
    } \
} while (0)


union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
};

#endif
