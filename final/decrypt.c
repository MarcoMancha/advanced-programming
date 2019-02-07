/*----------------------------------------------------------------

*

* Programación avanzada: Exámen Final

* Fecha: 27-Nov-2018

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

	if (argc != 3) {
		printf("usage: %s origin destination\n",argv[0]);
		return -1;
	}

	if ( (fd_in = open(argv[1], O_RDONLY)) < 0 ) {
		perror(argv[0]);
		return -1;
	}

	if ( (fd_out = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT , 0666)) < 0 ) {
		perror(argv[0]);
		return -1;
	}

	unsigned char buffer;
	ssize_t nbytes;
	int i,p;

	while ( (nbytes = read(fd_in, &buffer, 1)) != 0 ) {
			buffer = (buffer << 4) | (buffer >> 4);
			write(fd_out, &buffer, nbytes);
	}

	printf("Done\n");

	close(fd_in);
	close(fd_out);
	return 0;
}
