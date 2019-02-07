#include "header.h"

void cent() {
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

	while (1) {
		printf("Otaku de 100 esperando billetes\n");
		sem_wait(semid, CENTSEM);
		printf("Otaku de 100 ha comprado sus pokis\n");
		sem_signal(semid, BENJI);
		sleep(5);
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

	cent();
	return 0;
}
