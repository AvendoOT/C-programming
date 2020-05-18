// koristio sam skicu rješenja danu na
// http://www.zemris.fer.hr/predmeti/os/pripreme/z1b.html

#include <stdio.h>
#include <signal.h>

#define N 6

int OZNAKA_CEKANJA[N];
int PRIORITET[N];
int TEKUCI_PRIORITET;

int sig[] = {SIGUSR1, SIGUSR2, SIGTERM, SIGSYS, SIGINT};

void zabrani_prekidanje() {
  int i;
  for (i = 0; i < N-1; i++) {
    sighold(sig[i]);
  }
}

void dozvoli_prekidanje() {
  int i;
  for (i = 0; i < N-1; i++) {
    sigrelse(sig[i]);
  }
}

void obrada_signala(int i) {

  	int j = 0;

  	switch(i) {

  			case(1):
          printf("   -  P  -  -  -  -  \n");
  			  for(j=1; j<=5; j++) {
  				      printf("   -  %d  -  -  -  -  \n", j);
  				      sleep(1);
  				}
  				printf("   -  K  -  -  -  -  \n");
  				break;

  			case(2):
          printf("   -  -  P  -  -  -  \n");
  			  for(j=1; j<=5; j++) {
  					printf("   -  -  %d  -  -  -  \n", j);
  					sleep(1);
  				}
  				printf("   -  -  K  -  -  -  \n");
  				break;

  			case(3):
          printf("   -  -  -  P  -  -  \n");
  			  for(j=1; j<=5; j++) {
  				      printf("   -  -  -  %d  -  -  \n", j);
  				          sleep(1);
  				}
  				printf("   -  -  -  K  -  -  \n");
  				break;

  			case(4):
          printf("   -  -  -  -  P  -  \n");
  			  for(j=1; j<=5; j++) {
  					     printf("   -  -  -  -  %d  -  \n", j);
  					     sleep(1);
  				}
  				printf("   -  -  -  -  K  -  \n");
  				break;

  			case(5):
          printf("   -  -  -  -  -  P  \n");
          for(j=1; j<=5; j++) {
            printf("   -  -  -  -  -  %d  \n", j);
  					sleep(1);
  				}
  				printf("   -  -  -  -  -  K  \n");
  				break;
  		}
}

void prekidna_rutina(int sig) {
  int n = -1; // razina ispod TEKUCI_PRIORITET
  int i, j;
	zabrani_prekidanje();

	switch(sig) {

		case(SIGUSR1):
      n = 1;
			printf("   -  X  -  -  -  -  \n");
			break;

		case(SIGUSR2):
      n = 2;
			printf("   -  -  X  -  -  -  \n");
			break;

		case(SIGTERM):
      n = 3;
			printf("   -  -  -  X  -  -  \n");
			break;

		case(SIGSYS):
      n = 4;
			printf("   -  -  -  -  X  -  \n");
			break;

		case(SIGINT):
      n = 5;
			printf("   -  -  -  -  -  X  \n");
			break;
	}

    OZNAKA_CEKANJA[n] = 1;
    do {
      i = 0;
      for(j=TEKUCI_PRIORITET+1; j<N; j++){
        if(OZNAKA_CEKANJA[j] == 1)
          i = j;
      }

      if(i > 0){
        OZNAKA_CEKANJA[i] = 0;
        PRIORITET[i] = TEKUCI_PRIORITET;
        TEKUCI_PRIORITET = i;
        dozvoli_prekidanje();
        obrada_signala(i);
        zabrani_prekidanje();
        TEKUCI_PRIORITET = PRIORITET[i];
      }
    }while (i > 0);
	dozvoli_prekidanje();
}

int main(void) {

  int i;

  sigset(SIGUSR1, prekidna_rutina);
  sigset(SIGUSR2, prekidna_rutina);
  sigset(SIGTERM, prekidna_rutina);
  sigset(SIGSYS, prekidna_rutina);
  sigset(SIGINT, prekidna_rutina);

  printf("\nProces obrade prekida, PID = %d", getpid());
	printf("\n   G  1  2  3  4  5  \n");

  // 20 sekundi program ceka na prekide
	for(i=0; i<=9; i++){
    sleep(1);
    printf("   %d  -  -  -  -  -  \n", i);
	}

	for(i=10; i<=20; i++){
		sleep(1);
		printf("  %d  -  -  -  -  -  \n", i);
	}

  printf("\nZavrsio osnovni program\n");

  return 0;
}
