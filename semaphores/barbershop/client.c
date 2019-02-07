/*----------------------------------------------------------------

*

* Programación avanzada: Problema del barbero.

* Fecha: 8-Octubre-2018

* Autor: A01206194 Marco Mancha

*

*--------------------------------------------------------------*/
#include "header.h"

void clientes(int tiempo) {
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
		mutex_wait(semid, MUTEX);
		if(b -> customers == b -> chairs + 1){
			mutex_signal(semid, MUTEX);
			printf("There are no chairs available client %i is going to sleep\n", getpid());
			sleep(tiempo);
		}else{
			printf("There are %d available chairs, client %i will walk into the barbershop\n",b->chairs + 1 - b->customers, getpid());
			b -> customers = b -> customers + 1;
			sem_signal(semid, CUSTOMER);
			mutex_signal(semid, MUTEX);
			printf("Client %i is going to wait for barber to get their hair cut\n", getpid());
			sem_wait(semid, BARBER);
			mutex_wait(semid, MUTEX);
			b -> customers = b -> customers -1;
			mutex_signal(semid, MUTEX);
			printf("Client %i is done and is going to sleep\n", getpid());
			sleep(tiempo);
		}
	}

	shmdt(b);
	exit(0);
}

int main(int argc, char* argv[]) {
	int amount = 0, i, clients,pid;

	if (argc != 3) {
		printf("usage: %s time_to_return number_of_clients\n", argv[0]);
		return -1;
	}

	amount = atoi(argv[1]);
	if (amount <= 0) {
		printf("%s: The time must be a positive integer.\n", argv[0]);
		return -1;
	}

	clients = atoi(argv[2]);
	if (clients <= 0) {
		printf("%s: The number of clients must be a positive integer.\n", argv[0]);
		return -1;
	}

	for (i = 0; i < clients; i++) {
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			return -1;
		} else if (pid == 0) {
			clientes(amount);
		} else {
		}
	}
	return 0;
}
