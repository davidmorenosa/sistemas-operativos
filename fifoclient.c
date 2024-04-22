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
    int end_process;
    char readbuf[80];
    char end_str[5];

    printf("FIFO_CLIENT: envía mensajes, de forma indefinida, para terminar presionar \"end\"\n");

    // Abrir el FIFO de escritura hacia el servidor
    fd = open(FIFO_FILE, O_WRONLY);

    // Abrir el FIFO de lectura desde el servidor
    fd2 = open(FIFO_FILE2, O_RDONLY);

    strcpy(end_str, "end");

    while (1) {
        printf("Ingrese cadena: ");

        // Leer desde la consola
        fgets(readbuf, sizeof(readbuf), stdin);

        // Enviar la cadena al servidor
        write(fd, readbuf, strlen(readbuf) + 1);

        // Comprobar si se ha ingresado la cadena de fin
        end_process = strcmp(readbuf, end_str);
        if (end_process == 0) {
            close(fd);
            close(fd2);
            break;
        }

        // Leer la respuesta del servidor desde el FIFO
        read(fd2, readbuf, sizeof(readbuf));
        printf("Respuesta del servidor: \"%s\"\n", readbuf);

        // Comprobar si se ha ingresado la cadena de fin
        end_process = strcmp(readbuf, end_str);
        if (end_process == 0) {
            close(fd);
            close(fd2);
            break;
        }
    }

    return 0;
}
