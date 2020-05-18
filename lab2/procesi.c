#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int ShmId; /* segment id */

int N,M;

unsigned long int *A, *b;

void proces (int i)
{
  A[i] = i;
  for (int k=0; k<M; k++) {
    *b += 1;
  }
}

int main (int argc, char *argv[])
{
	int i;
  if (argc !=3) {
		printf("Argument number error\n");
		exit(1);
	}
	N=atoi(argv[1]); // create N threads
	M=atoi(argv[2]); // increment M times

	ShmId = shmget (IPC_PRIVATE, sizeof(int) * (N + 1), 0600);

	if (ShmId == -1)
		exit (1);

	A = (unsigned long int *) shmat (ShmId, NULL, 0);
  b = A + N;
  *b = 0;
	for (i = 0; i < N; i++)
		if (fork () == 0) {
			proces (i);
			exit (0);
		}

	for (i = 0; i < N+1; i++)
		wait (NULL);

	printf ("%ld\n", *b);
	shmdt ((char *) A);
	shmctl (ShmId, IPC_RMID, NULL);

	return 0;
}
