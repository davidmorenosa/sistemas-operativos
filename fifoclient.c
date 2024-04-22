#include <stdio.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_FILE "SERV_CLI"
#define FIFO_FILE2 "CLI_SERV"
#define BUFFER_SIZE 80

void *receive_message(void *arg) {
    int fd;
    char readbuf[BUFFER_SIZE];

    while (1) {
        // Leer desde el FIFO del servidor
        fd = open(FIFO_FILE2, O_RDONLY);
        read(fd, readbuf, BUFFER_SIZE);
        printf("Mensaje recibido del servidor: %s", readbuf);
        close(fd);
    }

    pthread_exit(NULL);
}

int main() {
    int fd;
    char writebuf[BUFFER_SIZE];
    pthread_t tid;
    int end_process = 0;

    // Crear el hilo para recibir mensajes
    pthread_create(&tid, NULL, receive_message, NULL);

    printf("FIFO_CLIENT: Comienza la comunicación, escribe 'end' para terminar.\n");

    // Abrir el FIFO para escritura
    fd = open(FIFO_FILE, O_WRONLY);

    while (!end_process) {
        printf("Ingrese mensaje para enviar al servidor: ");
        fgets(writebuf, BUFFER_SIZE, stdin);

        // Escribir en el FIFO
        write(fd, writebuf, strlen(writebuf));

        // Verificar si el mensaje es 'end'
        end_process = strcmp(writebuf, "end\n") == 0;
    }

    // Cerrar extremo de escritura del FIFO del servidor
    close(fd);

    // Esperar a que el hilo termine antes de salir del programa
    pthread_join(tid, NULL);

    return 0;
}
