#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <math.h>
#include <values.h>
#include <stdlib.h>
#include <stdatomic.h>

int Id;
atomic_int* ULAZ;
atomic_int* BROJ;
int N,M;
unsigned long int *A, *b;


void udi_u_kriticni_odsjecak(void * p) {
  int k, j, i;
  i = *((int *)p);
  ULAZ[i] = 1;
  for (k=1; k < N; k++)
       if (BROJ[k] > BROJ[i]) {
         BROJ[i] = BROJ[k];
       }
  BROJ[i] ++;
  ULAZ[i] = 0;

  for (j=0; j < N; j++)
  {
      while (ULAZ[j] != 0);
      while (BROJ[j] != 0 && ((BROJ[j]) < (BROJ[i])
       || ((BROJ[j]) == (BROJ[i]) && j < i)));
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
	(void) shmdt((char *) ULAZ);
	(void) shmctl(Id, IPC_RMID, NULL);
	exit (0);
}

void proces(void * p) {
    int k, m, i;
    i = *((int *)p);
    A[i] = i;
    for (k = 0; k < M; k++)
    {
      *b += 1;
      udi_u_kriticni_odsjecak((void *)&i);
      for (m = 1; m <= 5; m++)
      {
        sleep(1);
      }
      izadi_iz_kriticnog_odsjecka((void *)&i);
     }
}


int main(int argc, char *argv[]) {

    int i;

    if (argc !=3) {
  		printf("Argument number error\n");
  		exit(1);
  	}

  	N=atoi(argv[1]); // create N threads
  	M=atoi(argv[2]); // increment M times

    Id = shmget(IPC_PRIVATE, sizeof(int)*(N+1), 0600);

    if (Id == -1)
      exit(1);

    //*A = 0;

    ULAZ = (int *) shmat(Id, NULL, 0);
    BROJ = (int *) shmat(Id, NULL, 0)+N;

    for (i = 0; i < N; i++)
    {
        ULAZ[i]=0;
        BROJ[i]=0;
    }

    sigset(SIGINT, brisi);

    A = (unsigned long int *) shmat (Id, NULL, 0);
    b = A + N;
    *b = 0;

    for (i = 0; i < N; i++)
      switch (fork()) {
          case 0:
            proces((void *)&i);
            exit(0);
          case -1:
            printf("Error\n");
            break;
          default:
            break;
      }

    for (i = 0; i < N+1; i++)
 		   wait (NULL);



    printf("%ld\n", *b);

    return 0;
}
