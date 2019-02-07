#include "header.h"
#include <stdio.h>
#include <stdlib.h>

void readers() {
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
		printf("Reader %i is trying to enter room\n", getpid());
		mutex_wait(semid, MUTEX);
		b -> readers = b -> readers + 1;
		if(b -> readers == 1){
			sem_wait(semid, ROOM,1);
			printf("Reader %i is opening room\n", getpid());
		}
		mutex_signal(semid, MUTEX);
		printf("Reader %i will start reading\n", getpid());
		file = fopen("prueba.txt", "r");
		if (!file) {
			perror("prueba.txt");
			exit(-1);
		}
		while (fscanf(file, "%i", &i) != EOF) {
			printf("Read by %i: %i\n",getpid(), i);
		}
		fclose(file);
		printf("Reader %i stops reading\n", getpid());
		mutex_wait(semid, MUTEX);
		b -> readers = b -> readers - 1;
		if(b -> readers == 0){
			sem_signal(semid, ROOM,1);
			printf("Readers leaving room\n");
		}
		mutex_signal(semid, MUTEX);
		sleep(5);
	}

	shmdt(b);
	exit(0);
}

int main(int argc, char* argv[]) {
	int amount = 0, i,pid;

	if (argc != 2) {
		printf("usage: %s number_of_readers\n", argv[0]);
		return -1;
	}

	amount = atoi(argv[1]);
	if (amount <= 0) {
		printf("%s: Readers must be a positive integer.\n", argv[0]);
		return -1;
	}

	for (i = 0; i < amount; i++) {
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			return -1;
		} else if (pid == 0) {
			readers();
		} else {
		}
	}
	return 0;
}
