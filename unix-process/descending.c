/*----------------------------------------------------------------

*

* Programación avanzada: Creación de procesos.

* Fecha: 16-Sep-2018

* Autor: A01206194 Marco Mancha

*

*--------------------------------------------------------------*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void child_process(int level, int level_max, char *program) {
	int pid, time;

	for(int j=0 ; j<level ; j++){
		printf("\t");
	}

	printf("PPID = %i PID = %i NIVEL = %i\n", getppid(), getpid(), level);

	if (level < level_max) {
		level++;
		for(int i = 0 ; i < level ; i++){
			if ( (pid = fork()) < 0 ) {
				perror(program);
				exit(-1);
			} else if (pid == 0) {
				child_process(level,level_max, program);
			} else {
				wait(NULL);

				if(level > 0){
					sleep(1);
				}

				exit(0);
			}
		}

	}
}

int main(int argc, char* argv[]) {
	int i = 0;
	float num;
	int numero;

	if (argc != 2) {
		printf("usage: %s number_of_level\n", argv[0]);
		return -1;
	}

	num = atof(argv[1]);

	if (num < 1 || num != (int)num) {
		printf("%s: the parameter must be a positive integer greater than zero\n", argv[0]);
		return -1;
	}

	numero = num;

	child_process(i, numero, argv[0]);
	return 0;
}
