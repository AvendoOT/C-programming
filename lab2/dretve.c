#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int N, M;
unsigned long int A;

void *dretva () {
	for (int k=0; k<M; k++) {
		A++;
	}
}

int main (int argc, char *argv[]) {

	int i;
	pthread_t *t=NULL;

	if (argc !=3) {
		printf("Argument number error\n");
		exit(1);
	}

	A=0;
	N=atoi(argv[1]); // create N threads
	M=atoi(argv[2]); // increment M times

	t=(pthread_t*) malloc (N*sizeof(pthread_t));

	if(!t) {
		printf("Cannot allocate memory\n");
		exit(0);
	}

	for (i=0;i<N;++i) {
		if (pthread_create(t+i, NULL, dretva, NULL)) {
			printf("Cannot create thread\n");
			exit(0);
		}
	}

	for (i=0; i<N; ++i) {
		pthread_join(t[i],NULL);
	}

	printf ("%ld\n", A);
	return 0;
}
