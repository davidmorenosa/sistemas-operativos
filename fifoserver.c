/* Filename: fifoserver.c */
/* Servidor named pipe (FIFO) */

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_FILE "SERV_CLI"

#define FIFO_FILE2 "CLI_SERV"

int main() {
	
   //descriptor de acrhivo del named pipe:	
   int fd;
   
   //buffer del lectura:
   char readbuf[80];
   
   //cadena de fin de comunicación:
   char end[10];
   
   //terminar lectura?
   int to_end;
   
   //bytes leídos
   int read_bytes;
   
   /* crear el named pipe (FIFO) si no existe */
   mknod(FIFO_FILE, S_IFIFO|0640, 0);
   
   strcpy(end, "end");
   
   while(1) {
	   
      fd = open(FIFO_FILE, O_RDONLY);
	  
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