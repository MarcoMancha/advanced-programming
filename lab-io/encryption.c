/*----------------------------------------------------------------

*

* Programación avanzada: Cifrado de un archivo

* Fecha: 22-Agosto-2018

* Autor: A01206194 Marco Mancha

*

*--------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
  int fd_in, fd_out;

	if (argc != 4) {
		if(argc > 1){
			if( isdigit(*argv[1]) != 0) {
				printf("usage: encryption number origin destination\n");
			}else{
				printf("encryption: the first parameter must be a positive integer number.\n");
			}
		}else{
			printf("usage: encryption number origin destination\n");
		}
		return -1;
	}

	if( isdigit(*argv[1]) == 0) {
		printf("encryption: the first parameter must be a positive integer number.\n");
		return -1;
	}

	if ( (fd_in = open(argv[2], O_RDONLY)) < 0 ) {
		perror(argv[0]);
		return -1;
	}

	if ( (fd_out = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT )) < 0 ) {
		perror(argv[0]);
		return -1;
	}

	int number = atoi(argv[1]);
	char buffer[128];
	char new_buffer[128];
	ssize_t nbytes;
	int i,p;

	while ( (nbytes = read(fd_in, buffer, sizeof(buffer))) != 0 ) {
		if(nbytes == 128){
			for(i=0 ; i < 128 ; i++){
				p = i - number;
				if(p < 0){
					p = p + 128;
				}
				new_buffer[p] = buffer[i];
			}
			write(fd_out, new_buffer, nbytes);
		}else{
			write(fd_out, buffer, nbytes);
		}
	}

	printf("Done\n");

	close(fd_in);
	close(fd_out);
	return 0;
}
