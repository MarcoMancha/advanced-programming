/*----------------------------------------------------------------

*

* Programación avanzada: Problema del barbero.

* Fecha: 8-Octubre-2018

* Autor: A01206194 Marco Mancha

*

*--------------------------------------------------------------*/
#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define	CUSTOMER 		0
#define BARBER		1
#define MUTEX 		2

struct buffer {
	int customers;
	int chairs;
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
