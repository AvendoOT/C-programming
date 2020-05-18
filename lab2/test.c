#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <math.h>
#include <values.h>
#include <stdlib.h>

int Id;
int* TRAZIM;
int* BROJ;
int n;


void udi_u_kriticni_odsjecak(void * p)
{
   int k, j, i;
   i = *((int *)p);
   TRAZIM[i] = 1;
   for (k=1; k < n; k++)
        if (BROJ[k] > BROJ[i]) BROJ[i] = BROJ[k];
   BROJ[i] ++;
   TRAZIM[i] = 0;

   for (j=0; j < n; j++)
   {
       while (TRAZIM[j] != 0);
       while (BROJ[j] != 0 && ((BROJ[j]) < (BROJ[i]) || ((BROJ[j]) == (BROJ[i]) && j < i)));

}
}

void izadi_iz_kriticnog_odsjecka( void * p)
{
    int i;
    i = *((int *)p);
    (BROJ[i]) = 0;
    usleep(10);
}


void brisi(int sig)
{
	(void) shmdt((char *) BROJ);
	(void) shmdt((char *) TRAZIM);
	(void) shmctl(Id, IPC_RMID, NULL);
	exit (0);
}

void ispis(void * p)
{
    int k, m, i;
    i = *((int *)p);
    for (k = 1; k <=5; k++)
    {
      udi_u_kriticni_odsjecak((void *)&i);
      for (m = 1; m <= 5; m++)
      {
        printf("Dretva: %d, K.O. br: %d (%d/5)\n", i+1, k, m);
        sleep(1);
              }
      izadi_iz_kriticnog_odsjecka((void *)&i);
     }
}


int main(int argc, char *argv[])
{
    int i;
    n= atoi(argv[1]);
   Id = shmget(IPC_PRIVATE, sizeof(int)*100, 0600);

   if (Id == -1)
      exit(1);

   TRAZIM = (int *) shmat(Id, NULL, 0);
   BROJ = (int *) shmat(Id, NULL, 0)+n;

   for (i = 0; i < n; i++)
    {
        TRAZIM[i]=0;
        BROJ[i]=0;
    }

   sigset(SIGINT, brisi);

for (i = 0; i < n; i++)
   switch (fork()) {
   case 0:
      ispis((void *)&i);
      exit(0);
   case -1:
       printf("Greska pri stvaranju procesa!\n");
       break;
   default:
      break;
   }

while (i--) wait (NULL);


   return 0;
}
