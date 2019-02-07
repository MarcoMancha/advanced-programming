#include "header.h"

void benji1() {
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

	while(1) {
		sem_wait(semid, TWENTY);
		mutex_wait(semid, MUTEX);
			if(b -> isFifty == 0){
				b -> isFifty = 1;
				sem_signal(semid, CENTSEM);
			}else if(b -> isCent == 0){
				b -> isCent = 1;
				sem_signal(semid, FIFTYSEM);
			}else{
				b -> isTwenty = 0;
			}
			mutex_signal(semid, MUTEX);
			sleep(5);
	}
	shmdt(b);
	exit(0);
}

void benji2() {
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

	while(1) {
		sem_wait(semid, FIFTY);
		mutex_wait(semid, MUTEX);
			if(b -> isTwenty == 0){
				b -> isTwenty = 1;
				sem_signal(semid, CENTSEM);
			}else if(b -> isCent == 0){
				b -> isCent = 1;
				sem_signal(semid, TWENTYSEM);
			}else{
				b -> isFifty = 0;
			}
			mutex_signal(semid, MUTEX);
			sleep(5);
	}
	shmdt(b);
	exit(0);
}

void benji3() {
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

	while(1) {
		sem_wait(semid, CENT);
		mutex_wait(semid, MUTEX);
			if(b -> isFifty == 0){
				b -> isFifty = 1;
				sem_signal(semid, TWENTYSEM);
			}else if(b -> isTwenty == 0){
				b -> isTwenty = 1;
				sem_signal(semid, FIFTYSEM);
			}else{
				b -> isCent = 0;
			}
			mutex_signal(semid, MUTEX);
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


	for (i = 0; i < 3; i++) {
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			return -1;
		} else if (pid == 0) {
			if(i == 0){
				benji1();
			}else if(i == 1){
				benji2();
			}
			else{
				benji3();
			}
		} else {
		}
	}

	if ( (key = ftok("/dev/null", 200)) == (key_t) -1 ) {
		perror("ftok");
		exit(-1);
	}

	if ( (semid = semget(key, 3, 0666))  < 0 ) {
		perror("semget");
		exit(-1);
	}

	srand( getpid() );
	while(1){
		i = rand() % 3;
		sem_wait(semid, BENJI);
		if(i == 0){
			printf("Benji ha puesto 20 y 50\n");
			sem_signal(semid, TWENTY);
			sem_signal(semid, FIFTY);
		}else if(i==1){
			printf("Benji ha puesto 50 y 100\n");
			sem_signal(semid, FIFTY);
			sem_signal(semid, CENT);
		}else{
			printf("Benji ha puesto 20 y 100\n");
			sem_signal(semid, TWENTY);
			sem_signal(semid, CENT);
		}
		sleep(5);
	}
	return 0;
}
