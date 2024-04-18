/* fifoclient.c */
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
   int stringlen;
   char readbuf[80];
   
   char end_str[5];
   
   printf("FIFO_CLIENT: envia mensajes, de forma indefinida, para terminar presionar \"end\"\n");
   fd = open(FIFO_FILE, O_CREAT|O_WRONLY);
   
   strcpy(end_str, "end");
   
   while (1) {
	   
      printf("Ingrese cadena: ");
	  
      fgets(readbuf, sizeof(readbuf), stdin);
	  
      stringlen = strlen(readbuf);
	  
      readbuf[stringlen - 1] = '\0';
	  
      end_process = strcmp(readbuf, end_str);
      
      //printf("end_process is %d\n", end_process);
      if (end_process != 0) {
		  
         write(fd, readbuf, strlen(readbuf));
         printf("Cadena enviada: \"%s\" con longitud is %d\n", readbuf, (int)strlen(readbuf));
      } else {
         write(fd, readbuf, strlen(readbuf));
         printf("cadena enviada: \"%s\" con longitud %d\n", readbuf, (int)strlen(readbuf));
         close(fd);
         break;
      }
   }
   
   return 0;
}