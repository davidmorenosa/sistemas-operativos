/* Filename: fifoserver.c */
/* Servidor named pipe (FIFO) bidireccional */

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_FILE "SERV_CLI"

int main() {
    int fd;
    char readbuf[80];
    char end[10];
    int to_end;
    int read_bytes;

    mknod(FIFO_FILE, S_IFIFO | 0640, 0);
    strcpy(end, "end");

    while (1) {
        fd = open(FIFO_FILE, O_RDWR); // Abre el FIFO en modo lectura y escritura

        printf("Ingrese cadena (\"end\" para finalizar): ");
        scanf("%s", readbuf);

        write(fd, readbuf, strlen(readbuf));
        printf("Cadena enviada: \"%s\" con longitud %d\n", readbuf, (int)strlen(readbuf));

        read_bytes = read(fd, readbuf, sizeof(readbuf));
        readbuf[read_bytes] = '\0';

        printf("Cadena recibida: \"%s\" con longitud %d\n", readbuf, (int)strlen(readbuf));

        to_end = strcmp(readbuf, end);
        if (to_end == 0) {
            close(fd);
            break;
        }
    }

    return 0;
}
