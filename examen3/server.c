/*----------------------------------------------------------------
*
* Programación avanzada: Tercer exámen
* Fecha: 13-Noviembre-2018
* Autor: A01206194 Marco Mancha
*
*--------------------------------------------------------------*/

#include "header.h"
#include <string.h>
#include <time.h>

int val = 0;
char *filename;
char *ip_c;


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void escribe_conexion(){
	FILE* file;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	pthread_mutex_lock(&mutex);
	file = fopen(filename, "a");
	if (!file) {
		perror(filename);
		exit(-1);
	}
	fprintf(file, "---------------INICIO---------------------------\n");
	fprintf(file,"Conection time: %d-%d-%d %d:%d:%d Direccion: %s\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, ip_c);
	fprintf(file, "---------------INICIO---------------------------\n");
	fclose(file);
	pthread_mutex_unlock(&mutex);
}

void escribe_resultado(int code, int x, int y){
	FILE* file;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	file = fopen(filename, "a");
	if (!file) {
		perror(filename);
		exit(-1);
	}
	if(code != 301){
		fprintf(file, "---------------RESULTADO---------------------------\n");
		fprintf(file,"Conection time: %d-%d-%d %d:%d:%d Direccion: %s Valor anterior: %i Valor actual: %i\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, ip_c,x,y);
		fprintf(file, "---------------RESULTADO---------------------------\n");
		fclose(file);
	}else{
		fprintf(file, "---------------RESULTADO---------------------------\n");
		fprintf(file,"Conection time: %d-%d-%d %d:%d:%d Direccion: %s CONEXION CERRADA\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, ip_c);
		fprintf(file, "---------------RESULTADO---------------------------\n");
		fclose(file);
	}
}

void* serves_client(void *param) {
	int nsfd = *( (int*) param);
	int code, value;
	int band = 0;

	do {
		read(nsfd, &code, sizeof(code));
		switch(code){
			case 101:
				pthread_mutex_lock(&mutex);
				write(nsfd, &val, sizeof(val));
				escribe_resultado(code, val, val);
				pthread_mutex_unlock(&mutex);
				break;
			case 201:
				pthread_mutex_lock(&mutex);
				read(nsfd, &value, sizeof(value));
				escribe_resultado(code, val, value);
				val = value;
				band = 1;
				write(nsfd, &band, sizeof(band));
				pthread_mutex_unlock(&mutex);
				break;
			case 301:
				escribe_resultado(code, 0, 0);
				printf("\nFIN DE CONEXION\n");
				break;
			}
	} while (code != 301);
	close(nsfd);
	pthread_exit(NULL);
}

void server(char* ip, int port, char* program, char* filename) {
	int sfd, pid,nsfd;
	struct sockaddr_in server_info;
	struct sockaddr_in client_info;
	pthread_t thread_id;

	if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror(program);
		exit(-1);
	}

	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = inet_addr(ip);
	server_info.sin_port = htons(port);
	if ( bind(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
		perror(program);
		exit(-1);
	}

	listen(sfd, 1);
	while (1) {
		int len = sizeof(client_info);
		if ( (nsfd = accept(sfd, (struct sockaddr *) &client_info, &len)) < 0 ) {
			perror(program);
			exit(-1);
		}
		ip_c = inet_ntoa(client_info.sin_addr);
		escribe_conexion(ip_c);
		pthread_create(&thread_id, NULL, serves_client, ((void *) &nsfd));
	}
}

int main(int argc, char* argv[]) {
	char ip[15];
	int port;

	if (argc == 4) {
		strcpy(ip, argv[1]);
		port = atoi(argv[2]);
		filename = argv[3];
		if (port < 5000) {
						printf("%s: The port must be greater than 5000.\n", argv[0]);
						return -1;
					}
		strcpy(filename, argv[3]);
	}else{
		printf("usage: %s dir port logname\n", argv[0]);
		return -1;
	}

	server(ip, port, argv[0], filename);

	return 0;
}
