#include "header.h"
#include <time.h>

void filosofo(){

    int semid, i, k;
        key_t key;

        if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
            printf("Error al crear llave");
            exit(-1);
        }

        if ( (semid = semget(key, 4, 0666))  < 0 ) {
            printf("Error al crear semaforo");
            exit(-1);
        }

        while (1) {

            printf("Filosofo tratando de tomar dos tenedores.\n");

            sem_wait(semid, OCCUPIED, 2);
            printf("Filosofo tomando dos tenedores.\n");
            sem_signal(semid, FREESPACE, 2);

            sleep((rand() % 10) + 1);

            printf("Filosofo devuelve dos tenedores.\n");
            sem_signal(semid, OCCUPIED, 2);
        }
        exit(0);
}


int main(int argc, char* argv[]) {
	int amount = 0, semid, i, pid;
	key_t key;


    if (argc != 2) {
        printf("usage: %s [Numero de filosofos en la mesa]\n", argv[0]);
		return -1;
	}

	amount = atoi(argv[1]);

	for (i = 0; i < amount; i++) {
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			return -1;
		} else if (pid == 0) {
			filosofo();
		} else {
		}
	}
	return 0;
}
