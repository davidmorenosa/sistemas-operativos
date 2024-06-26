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
    int fd2;
    char readbuf[80];
    char end[10];
    int to_end;

    // Crear los FIFOs si no existen
    mknod(FIFO_FILE, S_IFIFO|0640, 0);
    mknod(FIFO_FILE2, S_IFIFO|0640, 0);

    strcpy(end, "end");

    while(1) {
        // Abrir el FIFO de lectura del cliente
        fd = open(FIFO_FILE, O_RDONLY);

        // Leer desde el FIFO
        read(fd, readbuf, sizeof(readbuf));

        printf("Cadena recibida del cliente: \"%s\"\n", readbuf);

        // Comprobar si se ha recibido la cadena de fin
        to_end = strcmp(readbuf, end);
        if (to_end == 0) {
            close(fd);
            break;
        }

        // Cerrar el descriptor de archivo
        close(fd);

        printf("Ingrese cadena para enviar al cliente: ");
        fgets(readbuf, sizeof(readbuf), stdin);
        readbuf[strcspn(readbuf, "\n")] = 0; // Eliminar el carácter de nueva línea

        // Abrir el FIFO de escritura hacia el cliente
        fd2 = open(FIFO_FILE2, O_WRONLY);

        // Enviar la cadena al cliente
        write(fd2, readbuf, strlen(readbuf) + 1);

        // Comprobar si se ha ingresado la cadena de fin
        to_end = strcmp(readbuf, end);
        if (to_end == 0) {
            close(fd2);
            break;
        }

        // Cerrar el descriptor de archivo
        close(fd2);
    }

    return 0;
}
