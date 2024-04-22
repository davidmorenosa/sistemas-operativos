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
    int end_process;
    char readbuf[80];
    char end_str[5];

    printf("FIFO_CLIENT: envia mensajes, de forma indefinida, para terminar presionar \"end\"\n");

    // Abrir el FIFO de escritura
    fd = open(FIFO_FILE, O_WRONLY);

    strcpy(end_str, "end");

    while (1) {
        printf("Ingrese cadena: ");

        // Leer desde la consola
        scanf("%s", readbuf);

        // Enviar la cadena al servidor
        write(fd, readbuf, strlen(readbuf) + 1);

        // Comprobar si se ha ingresado la cadena de fin
        end_process = strcmp(readbuf, end_str);
        if (end_process == 0) {
            close(fd);
            break;
        }
    }

    return 0;
}
