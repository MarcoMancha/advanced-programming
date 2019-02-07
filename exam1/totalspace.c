/*----------------------------------------------------------------

*

* Programación avanzada: Primer Examen PArcial

* Fecha: 4-Septiembre-2018

* Autor: A01206194 Marco Mancha

*

*--------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

int list(char* directory, char* program) {
 int fd, min, max, total = 0, files = 0;
 char filename[PATH_MAX + NAME_MAX + 1];
 DIR *dir;
 off_t number;
 struct dirent *dir_entry;
 struct stat ss;
 min = INT_MAX;
 max = INT_MIN;

 if ( (dir = opendir(directory)) == NULL ) {
	 perror(program);
	 exit(-1);
 }

 while ( (dir_entry = readdir(dir)) != NULL ) {
	 if (strcmp(dir_entry->d_name, ".") != 0 && strcmp(dir_entry->d_name, "..") != 0) {
		 sprintf(filename, "%s/%s" , directory, dir_entry->d_name);
		 lstat(filename, &ss);
   number = ss.st_size;
    if(S_ISDIR(ss.st_mode)){
     total = total + list(filename, program);
    }
	 }
 }

 rewinddir(dir);

 while ( (dir_entry = readdir(dir)) != NULL ) {
	 if (strcmp(dir_entry->d_name, ".") != 0 && strcmp(dir_entry->d_name, "..") != 0) {
		 sprintf(filename, "%s/%s" , directory, dir_entry->d_name);
		 lstat(filename, &ss);
   number = ss.st_size;
    if(S_ISREG(ss.st_mode)){
     files++;
     if (min > number) {
      min = number;
     }
     if (max < number) {
      max = number;
     }
     total = total + number;
    }
	 }
 }

 printf("\nDirectory: %s\n",directory);
 printf("Occupied space: %d Total files: %d \n",total,files);
 printf("Min size: %d Max size: %d \n",min,max);

 close(fd);

 return total;
}
 int main(int argc, char* argv[]) {
	char dir_name[NAME_MAX + 1];
	char *directory;

	if (argc != 2) {
		fprintf(stderr, "usage: %s directory\n", argv[0]);
		return -1;
	}

	list(argv[1], argv[0]);
	return 0;
}
