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

    printf("FIFO_CLIENT: Comienza la comunicación, escribe 'end' para terminar.\n");
    
    // Abrir el FIFO para escritura
    fd = open(FIFO_FILE, O_WRONLY);

    while (1) {
        printf("Ingrese mensaje para enviar al servidor: ");
        fgets(writebuf, BUFFER_SIZE, stdin);

        // Escribir en el FIFO
        write(fd, writebuf, strlen(writebuf));

        // Verificar si el mensaje es 'end'
        end_process = strcmp(writebuf, end_str);
        if (end_process == 0) {
            close(fd);
            break;
        }

        // Leer desde el FIFO del servidor
        fd = open(FIFO_FILE2, O_RDONLY);
        read(fd, readbuf, BUFFER_SIZE);
        printf("Mensaje recibido del servidor: %s", readbuf);
        close(fd);
    }

    return 0;
}
