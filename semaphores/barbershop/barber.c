/*----------------------------------------------------------------

*

* Programación avanzada: Problema del barbero.

* Fecha: 8-Octubre-2018

* Autor: A01206194 Marco Mancha

*

*--------------------------------------------------------------*/
#include "header.h"

void barber() {
	int semid, shmid, i, j, k, blk_size, tamanio;
	key_t key;
	struct buffer *b;

	if ( (key = ftok("/dev/null", 200)) == (key_t) -1 ) {
		perror("ftok");
		exit(-1);
	}

	if ( (semid = semget(key, 3, 0666))  < 0 ) {
		perror("semget");
		exit(-1);
	}

	if ( (shmid = shmget(key, sizeof(struct buffer), 0666)) < 0 ) {
		perror("shmid");
		exit(-1);
	}
	b = (struct buffer *) shmat(shmid, (void *) 0, 0);

	while (1) {
		printf("Barber sleeping waiting for customer\n");
		sem_wait(semid, CUSTOMER);
		mutex_wait(semid, MUTEX);
		printf("Barber has awaken\n");
		sem_signal(semid, BARBER);
		mutex_signal(semid, MUTEX);
		printf("Barber is cutting hair\n");
	}
	shmdt(b);
	exit(0);
}

int main(int argc, char* argv[]) {
	int amount = 0, semid, i, pid;
	key_t key;

	if (argc != 1) {
		printf("usage: %s\n", argv[0]);
		return -1;
	}

	barber();
	return 0;
}
