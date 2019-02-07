/*----------------------------------------------------------------

*

* Programación avanzada: Manejando señales

* Fecha: 23-Sep-2018

* Autor: A01206194 Marco Mancha

*

*--------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>
#include <signal.h>

int cont = 0;
int n = 0;
int bandera = 0;

void handler(int signal) {
    switch(signal) {
        case SIGUSR1 :
            if(bandera == 0){
                printf("SIGUSR1 - SIGUSR2 is now ignored\n");
                bandera = 1;
            }else{
                printf("SIGUSR1 - SIGUSR2 is now accepted\n");
                bandera = 0;
            }
            break;
        case SIGUSR2 :
            if(bandera == 0){
                cont++;
                if(cont == n){
                    printf("Exiting...\n");
                    kill(getpid(),SIGTERM);
                }
                printf("SIGUSR2 - cont = %d n = %d\n", cont, n);
            }
            break;
    }
}

int main(int argc, char* argv[]) {
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);

    float num;

    if (argc > 2) {
		printf("usage: %s [n] \n", argv[0]);
		return -1;
	}

    if (argc == 1){
        n = 10;
        num = 10.0;
    }else{
        n = atoi(argv[1]);
        num = atof(argv[1]);
    }

	if (n < 1 || num != n) {
		printf("%s: the parameter must be a positive integer greater than zero.\n", argv[0]);
		return -1;
	}

    while (1) {
        pause();
    }
}
