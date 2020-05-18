// koristio sam skicu rješenja danu na
// http://www.zemris.fer.hr/predmeti/os/pripreme/z1b.html

#include  <stdio.h>
#include  <signal.h>
#include  <stdlib.h>
#include  <time.h>

int pid = 0;

void prekidna_rutina(int sig) {
	kill(pid, SIGKILL);
	exit(0);
}

int main(int argc, char *argv[]){

	int timer, interrupt;
  int i;
	pid = atoi(argv[1]);

	sigset(SIGINT, prekidna_rutina);
	srand((unsigned)time(NULL));

	while(1){
    // random vrijeme cekanja I = [3,5]
		timer = rand() % (5-3+1)+ 3;
    for (i = 0; i < timer; i++) {
      sleep(1);
    }

    // biranje svih signala osim SIGINT-a I = [1,4]
		interrupt = rand () % (4-1+1)+1;

		switch(interrupt){
			case(1):
        kill(pid, SIGUSR1);
				break;
			case(2):
        kill(pid, SIGUSR2);
				break;
			case(3):
        kill(pid, SIGTERM);
				break;
			case(4):
        kill(pid, SIGSYS);
				break;
		}
	}
	return 0;
}
