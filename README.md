/*
$ ipcs

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages

------ Shared Memory Segments --------
key        shmid      owner      perms      bytes      nattch     status
0x00001001 13         ljh        666        1024       1

------ Semaphore Arrays --------
key        semid      owner      perms      nsems
0x00001002 23         ljh        666        1
0x00001003 24         ljh        666        1

$

$ ipcrm shm 13
resource(s) deleted
$

$ ipcrm sem 23 24
resource(s) deleted
$

*/
