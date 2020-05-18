// koristio sam skicu rješenja danu na
// http://www.zemris.fer.hr/predmeti/os/pripreme/z1a.html
#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED
#define PROSTI_BROJ_START 1000000001

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>

// globalno deklarirano da funkcije vide u svakom trenutku
unsigned long zadnjiProsti = PROSTI_BROJ_START;
int pauza = 0;
// zastavica za SIGINT zaustavljam/nastavljam
int flag = 0;

int prost(unsigned long broj) {
  unsigned long i, max;
  if (broj == 2) return 1;
  // ako je broj paran i razlicit od 2 - NIJE PROST
  if (broj % 2 == 0) return 0;
  max = sqrt(broj);
  for(i = 3; i <= max; i += 2) {
    if (broj % i == 0) return 0;
  }
  // broj JE PROST
  return 1;

}

void ispis() {
  printf("\nZadnja prosti broj : %lu", zadnjiProsti);
}

void periodicki_ispis(int sig) {
  ispis();
}

int postavi_pauzu(int sig) {
  if (flag == 0) {
    printf("\npauziram");
    flag = 1;
  } else {
    printf("\nnastavljam");
    flag = 0;
  }
  pauza = 1 - pauza;
  return pauza;
}

void prekini(int sig) {
  printf("\nUlovljen [SIGTERM]");
  ispis();
  printf("\n");
  exit(0);
}

int main(void) {
  struct itimerval t;
  unsigned long broj = PROSTI_BROJ_START;

  sigset(SIGTERM, prekini);
  // za sigalarm periodicki posao je ispis zadnjeg prostog broja
  sigset(SIGALRM, periodicki_ispis);
  sigset(SIGINT, postavi_pauzu); // ctrl-c
  // program ne hvata druge oblike prekida signala
  // npr SIGQUIT izbacuje jezgru i prekida program
  // bez ikakvog hvatanja

  t.it_value.tv_sec = 0;
	t.it_value.tv_usec = 300000;

	t.it_interval.tv_sec = 0;
	t.it_interval.tv_usec = 300000;

  setitimer(ITIMER_REAL, &t, NULL);

  do {
    if (prost(broj) == 1) {
      zadnjiProsti = broj;
    }
    broj++;
    while (pauza == 1) pause();
  } while(1);

  return 0;
}
