/*----------------------------------------------------------------

*

* Programación avanzada: Problema del barbero.

* Fecha: 8-Octubre-2018

* Autor: A01206194 Marco Mancha

*

*--------------------------------------------------------------*/
#include "header.h"

int main(int argc, char* argv[]) {
	int semid, shmid, i, amount;
	key_t key;

	if (argc != 2) {
		printf("usage: %s chairs\n", argv[0]);
		return -1;
	}

	amount = atoi(argv[1]);
	if (amount <= 0) {
		printf("%s: The chairs must be a positive integer.\n", argv[0]);
		return -1;
	}

	if ( (key = ftok("/dev/null", 200)) == (key_t) -1 ) {
		perror("ftok");
		return -1;
	}

	if ( (semid = semget(key, 3, 0666 | IPC_CREAT))  < 0 ) {
		perror("semget");
		return -1;
	}

	semctl(semid, CUSTOMER, SETVAL, 0);
	semctl(semid, BARBER, SETVAL, 0);
	semctl(semid, MUTEX, SETVAL, 1);

	if ( (shmid = shmget(key, sizeof(struct buffer), 0666 | IPC_CREAT)) < 0 ) {
		semctl(semid, 0, IPC_RMID, 0);
		perror("shmget");
		return -1;
	}

	struct buffer *b = (struct buffer *) shmat(shmid, (void *) 0, 0);
	b->customers = 0;
	b->chairs = amount;
	shmdt(b);

	return 0;
}
