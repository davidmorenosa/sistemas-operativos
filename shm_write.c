/* shm_write.c */

#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#define BUF_SIZE 1024

#define SHM_KEY 0x1234

/* Estructura para manejar el bufer de escritura en memoria */
struct shmseg {
   int cnt;
   int complete;
   char buf[BUF_SIZE];
};


int fill_buffer(char * bufptr, int size);

int main(int argc, char *argv[]) {
	
   int shmid, numtimes;
   struct shmseg *shmp;
   char *bufptr;
   int spaceavailable;
   shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0644|IPC_CREAT);
   if (shmid == -1) {
      perror("Error en memoria compartida");
      return 1;
   }
   
   // Vincularse al segmento de memoria a través de un apuntador:
   shmp = shmat(shmid, NULL, 0);
   if (shmp == (void *) -1) {
      perror("Error vinculandose a la memoria compartida");
      return 1;
   }
   
   /* Transfiere bloques de datos del buffer al segmento de memoria */
   bufptr = shmp->buf;
   spaceavailable = BUF_SIZE;
   
   for (numtimes = 0; numtimes < 5; numtimes++) {
      shmp->cnt = fill_buffer(bufptr, spaceavailable);
      shmp->complete = 0;
      printf("Proceso que escribe : Escritura en memoria compartida: Se escribieron %d bytes\n", shmp->cnt);
      bufptr = shmp->buf;
      spaceavailable = BUF_SIZE;
      sleep(3);
   }
   printf("Proceso que escribe: Se escribio %d veces \n", numtimes);
   shmp->complete = 1;

   /* se va a desvincular de la memoria compartida */
   if (shmdt(shmp) == -1) {
      perror("shmdt");
      return 1;
   }

   /*   */
   if (shmctl(shmid, IPC_RMID, 0) == -1) {
      perror("shmctl");
      return 1;
   }
   printf("Proceso de escritura: completo\n");
   return 0;
}

/* método para llenar el bufer de escritura */
int fill_buffer(char * bufptr, int size) {
   static char ch = 'A';
   int filled_count;
   
   //printf("size is %d\n", size);
   memset(bufptr, ch, size - 1);
   bufptr[size-1] = '\0';
   if (ch > 122)
   ch = 65;
   if ( (ch >= 65) && (ch <= 122) ) {
      if ( (ch >= 91) && (ch <= 96) ) {
         ch = 65;
      }
   }
   filled_count = strlen(bufptr);
   
   //printf("buffer count is: %d\n", filled_count);
   //printf("buffer filled is:%s\n", bufptr);
   ch++;
   return filled_count;
}