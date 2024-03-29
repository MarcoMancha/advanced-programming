#include "header.h"

int main(int argc, char* argv[]) {
	int semid, shmid, i, amount;
	key_t key;

	if (argc != 1) {
		printf("usage: %s\n", argv[0]);
		return -1;
	}

	if ( (key = ftok("/dev/null", 200)) == (key_t) -1 ) {
		perror("ftok");
		return -1;
	}

	if ( (semid = semget(key, 8, 0666 | IPC_CREAT))  < 0 ) {
		perror("semget");
		return -1;
	}

	semctl(semid, TWENTY, SETVAL, 0);
	semctl(semid, FIFTY, SETVAL, 0);
	semctl(semid, CENT, SETVAL, 0);
	semctl(semid, TWENTYSEM, SETVAL, 0);
	semctl(semid, FIFTYSEM, SETVAL, 0);
	semctl(semid, CENTSEM, SETVAL, 0);
	semctl(semid, BENJI, SETVAL, 1);
	semctl(semid, MUTEX, SETVAL, 1);

	if ( (shmid = shmget(key, sizeof(struct buffer), 0666 | IPC_CREAT)) < 0 ) {
		semctl(semid, 0, IPC_RMID, 0);
		perror("shmget");
		return -1;
	}

	struct buffer *b = (struct buffer *) shmat(shmid, (void *) 0, 0);
	b->isTwenty = 1;
	b->isFifty = 1;
	b->isCent = 1;
	shmdt(b);

	return 0;
}
