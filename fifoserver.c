#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_FILE "SERV_CLI"
#define FIFO_FILE2 "CLI_SERV"

int main() {
    int fd;
    char readbuf[80];
    char end[10];
    int to_end;

    // Crear el FIFO si no existe
    mknod(FIFO_FILE, S_IFIFO|0640, 0);

    strcpy(end, "end");

    while(1) {
        // Abrir el FIFO de lectura
        fd = open(FIFO_FILE, O_RDONLY);

        // Leer desde el FIFO
        read(fd, readbuf, sizeof(readbuf));

        printf("Cadena recibida: \"%s\"\n", readbuf);

        // Comprobar si se ha recibido la cadena de fin
        to_end = strcmp(readbuf, end);
        if (to_end == 0) {
            close(fd);
            break;
        }

        // Cerrar el descriptor de archivo
        close(fd);

        printf("Ingrese cadena para enviar al cliente: ");
        scanf("%s", readbuf);

        // Abrir el FIFO de escritura
        fd = open(FIFO_FILE2, O_WRONLY);

        // Enviar la cadena al cliente
        write(fd, readbuf, strlen(readbuf) + 1);

        // Comprobar si se ha ingresado la cadena de fin
        to_end = strcmp(readbuf, end);
        if (to_end == 0) {
            close(fd);
            break;
        }

        // Cerrar el descriptor de archivo
        close(fd);
    }

    return 0;
}
