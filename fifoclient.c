/* Filename: fifoclient.c */
/* Cliente named pipe (FIFO) bidireccional */

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_FILE "SERV_CLI"

int main() {
    int fd;
    int end_process;
    int stringlen;
    char readbuf[80];
    char end_str[5];

    printf("FIFO_CLIENT: envía mensajes, de forma indefinida, para terminar presionar \"end\"\n");
    fd = open(FIFO_FILE, O_RDWR); // Abre el FIFO en modo lectura y escritura

    strcpy(end_str, "end");

    while (1) {
        printf("Ingrese cadena (\"end\" para finalizar): ");
        scanf("%s", readbuf);

        stringlen = strlen(readbuf);
        readbuf[stringlen] = '\0';

        end_process = strcmp(readbuf, end_str);
        if (end_process != 0) {
            write(fd, readbuf, strlen(readbuf));
            printf("Cadena enviada: \"%s\" con longitud %d\n", readbuf, (int)strlen(readbuf));

            int read_bytes = read(fd, readbuf, sizeof(readbuf));
            readbuf[read_bytes] = '\0';
            printf("Cadena recibida: \"%s\" con longitud %d\n", readbuf, (int)strlen(readbuf));
        } else {
            write(fd, readbuf, strlen(readbuf));
            printf("Cadena enviada: \"%s\" con longitud %d\n", readbuf, (int)strlen(readbuf));
            close(fd);
            break;
        }
    }

    return 0;
}
