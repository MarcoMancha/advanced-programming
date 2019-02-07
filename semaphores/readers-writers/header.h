#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define	ROOM	0
#define MUTEX	1

struct buffer {
	int readers;
};

int sem_wait(int semid, int semnum, unsigned int val) {
 struct sembuf op;

 op.sem_num = semnum;
 op.sem_op = -val;
 op.sem_flg = 0;
 return semop(semid, &op, 1);
}
int sem_signal(int semid, int semnum, unsigned int val) {
 struct sembuf op;

 op.sem_num = semnum;
 op.sem_op = val;
 op.sem_flg = 0;
 return semop(semid, &op, 1);
}

int mutex_wait(int semid, int sem_num) {
 return sem_wait(semid, sem_num,1);
}

int mutex_signal(int semid, int sem_num) {
 return sem_signal(semid, sem_num,1);
}

#endif
