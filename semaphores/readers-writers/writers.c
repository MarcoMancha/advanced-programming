#include "header.h"
#include <stdio.h>
#include <stdlib.h>

void writer() {
	FILE* file;
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
		printf("Writer trying to enter room\n");
		sem_wait(semid, ROOM,1);
		printf("Writer in the room\n");
		file = fopen("prueba.txt", "a");
		if (!file) {
			perror("prueba.txt");
			exit(-1);
		}
		printf("Dame un numero: ");
		scanf("%i",&i);
		fprintf(file, "%i\n", i);
		printf("Writer writing on prueba.txt: %i\n",i);
		fclose(file);
		sem_signal(semid, ROOM,1);
		printf("Writer out of room\n");
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

	writer();
	return 0;
}
