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
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <limits.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>


#define ENVIO_ARCHIVO 101
#define DESPLEGAR_DIRECTORIO 102
#define FIN_CONEXION 103
#define HOLA 1
#define PERMISO_DENEGADO 201
#define ARCHIVO_NO_ENCONTRADO 202
#define ERROR_INTERNO 203
#define COMANDO_NO_CONOCIDO 204
#define RUTA_ES_DIRECTORIO 205
#define ENVIANDO_ARCHIVO 301
#define DIRECTORIO_NO_ENCONTRADO 206
#define RUTA_NO_DIRECTORIO 207
#define ENVIANDO_CONTENIDO_DIRECTORIO 302

#define DEFAULT_PORT    9999
#define DEFAULT_IP      "127.0.0.1"

void snd_msg(int sfd, int code, char* data) {
	long length = strlen(data);

	printf("\nenviando...\n");
	printf("codigo: %i\n", code);
	write(sfd, &code, sizeof(code));
	printf("lenght = %li\n", length);
	write(sfd, &length, sizeof(length));
	printf("text = %s\n", data);
	write(sfd, data, length * sizeof(char));
	printf("\n\n");
}

int rcv_msg(int sfd) {
	int code;
	long length, data_read;
	char* data;

	printf("recibiendo...\n");
	read(sfd, &code, sizeof(code));
	printf("codigo: %i\n", code);
	read(sfd, &length, sizeof(length));
	printf("lenght = %li\n", length);
	data = (char*) malloc(length * sizeof(char));
	data_read = read(sfd, data, length * sizeof(char));
	// data[length] = '\0';
	printf("text = %s\n", data);
	free(data);

	printf("\n\n");

	if (data_read != length) {
		return -1;
	} else {
		return code;
	}
}

#endif
