#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

void snd_msg(int sfd, int code, char* data) {
	long length = strlen(data);
	write(sfd, &code, sizeof(code));
	write(sfd, &length, sizeof(length));
	write(sfd, data, length * sizeof(char));
	printf("Enviando codigo: %i lenght = %li text = %s\n",code,length,data);
}
#endif
