/*----------------------------------------------------------------

*

* Programación avanzada: Proyecto Parcial

* Fecha: 18 Septiembre 2018

* Autor: A01206194 Marco Mancha

*

*--------------------------------------------------------------*/


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <limits.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>

void rotation(char* program, char* directory, char* filename, int number, int timeout) {
	DIR* dir;
	FILE* file;
	struct dirent** direntry;
	struct stat info;
	char current[PATH_MAX + NAME_MAX + 1];
	char reloc[PATH_MAX + NAME_MAX + 1];
	char past[PATH_MAX + NAME_MAX + 1];
	char archivo[PATH_MAX + NAME_MAX + 1];
	char* split_nombre[3];
	char *token;
	int j,cuenta,fd,i,n,x;


	getcwd(current, NAME_MAX);

	if ( (dir = opendir(directory)) == NULL ) {
		perror(program);
		exit(-1);
	}

	lstat(directory, &info);

	if(info.st_mode & S_IRUSR && info.st_mode & S_IWUSR){
	}
	else{
		printf("error: no read and write permission for working_directory\n");
	}

	sprintf(archivo, "%s/%s.log", directory, filename);
	file = fopen(archivo, "r");
	if (!file) {
		perror(archivo);
		exit(-1);
	}
	fclose(file);

	for(int i = 0 ; i <= number ; i++){
		sleep(timeout);
   	n = scandir(directory, &direntry,NULL,alphasort);
		x = n - 1;
	  while (x>=0) {
			if (strcmp(direntry[x]->d_name, ".") != 0 && strcmp(direntry[x]->d_name, "..") != 0) {
				token = strtok(direntry[x]->d_name, ".");
				j = 0;
				while (token != NULL)
				{
						split_nombre[j] = token;
						token = strtok(NULL, ".");
						j++;
				}
				if(strcmp(split_nombre[1], "log") != 0){
					cuenta = atoi(split_nombre[1]);
					cuenta++;
					sprintf(past, "%s/%s/%s.%s", current, directory, filename, split_nombre[1]);
					sprintf(reloc, "%s/%s/%s.%d", current, directory, filename,cuenta);
					rename(past,reloc);
				}
			}
	     x--;
	  }

		if(i == number){
			sprintf(past, "%s/%s/%s.%d", current, directory, filename, number+1);
			remove(past);
		}

		sprintf(past, "%s/%s/%s.log", current, directory, filename);
		sprintf(reloc, "%s/%s/%s.1", current, directory, filename);
		rename(past,reloc);

		sprintf(reloc, "%s/%s/%s.log", current, directory, filename);
		open(reloc, O_WRONLY | O_TRUNC | O_CREAT, 777);
	}

}

int main(int argc, char* argv[]) {
	char dir_name[NAME_MAX + 1];
	struct stat info;
	char *directory;
	int recursive = 0;
	int numero;
	int timeout;

	if (argc > 5 || argc < 5) {
		printf("usage: %s working_directory log_filename number_of_files timeout \n", argv[0]);
		return -1;
	}

	numero = atoi(argv[3]);
	timeout = atoi(argv[4]);

	if (numero < 1) {
		printf("%s: the number of rotations must be a positive integer greater than zero\n", argv[0]);
		return -1;
	}

	if (timeout < 1) {
		printf("%s: the timeout must be a positive integer greater than zero\n", argv[0]);
		return -1;
	}

	rotation(argv[0], argv[1], argv[2], numero, timeout);
	return 0;
}
