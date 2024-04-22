#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_FILE "SERV_CLI"
#define FIFO_FILE2 "CLI_SERV"
#define BUFFER_SIZE 80

int main() {
    int fd;
    char readbuf[BUFFER_SIZE];
    char writebuf[BUFFER_SIZE];
    int end_process;
    char end_str[] = "end";

    printf("FIFO_SERVER: Comienza la comunicación, escribe 'end' para terminar.\n");

    // Crear el FIFO si no existe
    mknod(FIFO_FILE, S_IFIFO | 0640, 0);

    while (1) {
        // Leer desde el FIFO del cliente
        fd = open(FIFO_FILE, O_RDONLY);
        read(fd, readbuf, BUFFER_SIZE);
        printf("Mensaje recibido del cliente: %s", readbuf);
        close(fd);

        // Verificar si el mensaje es 'end'
        end_process = strcmp(readbuf, end_str);
        if (end_process == 0) {
            break;
        }

        printf("Ingrese mensaje para enviar al cliente: ");
        fgets(writebuf, BUFFER_SIZE, stdin);

        // Abrir el FIFO para escritura
        fd = open(FIFO_FILE2, O_WRONLY);

        // Escribir en el FIFO
        write(fd, writebuf, strlen(writebuf));

        // Verificar si el mensaje es 'end'
        end_process = strcmp(writebuf, end_str);
        if (end_process == 0) {
            close(fd);
            break;
        }

        close(fd);
    }

    return 0;
}
