#include "header.h"

void a_serf(char* program) {
	int semid, i = 1;
	key_t key;

	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror(program);
		exit(-1);
	}

	if ( (semid = semget(key, 4, 0666))  < 0 ) { //identificador del semaforo, numero de semaforos asociados, y otra cosa
		perror(program);
		exit(-1);
	}

	printf("Serf %i trying to access the boat.\n", getpid());
	mutex_wait(semid, MUTEX);
	printf("Serf %i trying to get in the boat with %i space(s)\n", getpid(), semctl(semid, BARRIER, GETVAL, 0));
  sem_wait(semid, BARRIER, 1);

  if (semctl(semid, HACKERS, GETVAL, 0) == 3) {
    printf("Too many hackers, serf off boat!!!!!\n");
    sem_signal(semid, BARRIER, 1);
  }
	else if ( (semctl(semid, HACKERS, GETVAL, 0) == 1) && (semctl(semid, SERFS, GETVAL, 0) == 2)) {
		printf("Too many serfs, serf off boat!!!!!\n");
		sem_signal(semid, BARRIER, 1);
	}
  else{
    sem_signal(semid, SERFS, 1);
  	printf("Serf %i accepted in the boat.\n", getpid());
    printf("%i Hackers in the boat, %i Serfs in the boat\n", semctl(semid, HACKERS, GETVAL, 0), semctl(semid, SERFS, GETVAL, 0));
  }
  if(semctl(semid, BARRIER, GETVAL, 0) == 0) {
    sem_signal(semid, BARRIER, 4);
    printf("Boat departed %i Hackers in the boat, %i Serfs in the boat\n\n", semctl(semid, HACKERS, GETVAL, 0), semctl(semid, SERFS, GETVAL, 0));
		sleep(3);
    semctl(semid, HACKERS, SETVAL, 0);
    semctl(semid, SERFS  , SETVAL, 0);
  }

	mutex_signal(semid, MUTEX);


	exit(0);
}

int main(int argc, char* argv[]) {
	int amount = 0, semid, i, pid;
	key_t key;

	if (argc != 2) {
		printf("usage: %s amount\n", argv[0]);
		return -1;
	}

	amount = atoi(argv[1]);
	if (amount < 1) {
		printf("%s: The amount must be a positive number greater than zero.\n", argv[0]);
		return -1;
	}

	for (i = 0; i < amount; i++) {
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			return -1;
		} else if (pid == 0) {
			a_serf(argv[0]);
		} else {
		}
	}
	return 0;
}
