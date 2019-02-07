#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define	TWENTY 		0
#define FIFTY		1
#define CENT		2
#define BENJI		3
#define TWENTYSEM		4
#define FIFTYSEM		5
#define CENTSEM		6
#define MUTEX 		7

struct buffer {
	int isTwenty, isFifty, isCent;
};

int sem_wait(int semid, int sem_num) {
 struct sembuf op;

 op.sem_num = sem_num;
 op.sem_op = -1;
 op.sem_flg = 0;
 return semop(semid, &op, 1);
}

int sem_signal(int semid, int sem_num) {
 struct sembuf op;

 op.sem_num = sem_num;
 op.sem_op = 1;
 op.sem_flg = 0;
 return semop(semid, &op, 1);
}

int mutex_wait(int semid, int sem_num) {
 return sem_wait(semid, sem_num);
}

int mutex_signal(int semid, int sem_num) {
 return sem_signal(semid, sem_num);
}

#endif
