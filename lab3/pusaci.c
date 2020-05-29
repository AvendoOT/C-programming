// korišten pseudokod http://www.zemris.fer.hr/predmeti/os/pripreme/z3_semafori-pusaci.html i
// online lab3 priprema prof. Jelenkovica i video 'Problem pusaca cigareta'

#include <stdio.h>
#include <stdlib.h>
#include<sys/sem.h>
#include<time.h>
#include<sys/ipc.h>

int semId;

// kombinacije koje pusac NEMA
int  duhanSibice=1; // prvi pusac
int  duhanPapir=2; // drugi pusac
int  papirSibice=3; // treci pusac
int  prazanStol=0; // pocetno stanje trgovca


// uzmi identifikator semafora
void semGet(int n) {
	semId = semget(IPC_PRIVATE, n, 0600);
	if(semId == -1) {
		printf("Semafor nije identificiran\n");
		exit(1);
	}
}

// postavi vrijednost semaforu
int semSetVal(int semNum, int semVal) {
	return semctl(semId, semNum, SETVAL, &semVal);
}

// primjeni operacije na semafor
int semOp(int semNum, int semOp) {
	struct sembuf semBuf;
	semBuf.sem_num = semNum;
	semBuf.sem_op = semOp;
	semBuf.sem_flg = 0;
	return semop(semId, &semBuf, 1);
}

// makni set semafora
void semRemove(void) {
	(void) semctl(semId, 0, IPC_RMID, 0);
}


void pusac1() {
	while(1){
		semOp(duhanSibice,-1);
		printf("Pusac 1: uzima sastojke DUHAN i SIBICE\n");
    // sto je na stolu
		semSetVal(prazanStol,1);
	}
}

void pusac2() {
	while(1){
		semOp(duhanPapir, -1);
		printf("Pusac 2: uzima sastojke DUHAN i PAPIR\n");
		semSetVal(prazanStol,1);
	}
}

void pusac3() {
	while(1){
		semOp(papirSibice, -1);
		printf("Pusac 3: uzima sastojke PAPIR i SIBICE\n");
		semSetVal(prazanStol,1);
	}
}

void trgovac() {
	int proizvod1, proizvod2;
	while(1){
		semOp(prazanStol, -1);
		srand((unsigned)time(NULL));
		sleep(1);
		proizvod1=rand()%(3-0+1)+0;
		proizvod2=rand()%(3-0+1)+0;
		while(proizvod1==proizvod2){
			proizvod2=rand()%3;
		}

    // sve kombinacije proizvoda
	if(proizvod1==1 && proizvod2==2) {
		printf("Trgovac stavlja: SIBICE i PAPIR\n");
		semOp(papirSibice, 1);
	}
	else if(proizvod1==1 && proizvod2==3) {
		printf("Na stol stavljam DUHAN i PAPIR\n");
		semOp(duhanPapir, 1);
	} else {
	     	printf("Na stol stavljam DUHAN i SIBICE\n");
		semOp(duhanSibice, 1);
	}
  semOp(prazanStol, 1);
  }
}


int main(void) {

  int i;

  semGet(4);
  semOp(prazanStol, 1);

  printf("Pusac 1: ima PAPIR\n");
  printf("Pusac 2: ima SIBICE\n");
  printf("Pusac 3: ima DUHAN.\n");
  printf("\n");

  switch(fork()) {
        case -1:
          printf("Proces se ne moze stvoriti\n");
          exit(0);
        case  0:
          trgovac(1);
          exit(0);
        default:
          break;
  }

  switch(fork()) {
      case -1:
          printf("Proces se ne moze stvoriti\n");
          exit(0);
      case  0:
          pusac1();
          exit(0);
      default:
          break;
  }

  switch(fork()) {
      case -1:
          printf("Proces se ne moze stvoriti\n");
          exit(0);
      case  0:
          pusac2();
          exit(0);
      default:
          break;
  }

  switch(fork()) {
      case -1:
          printf("Proces se ne moze stvoriti\n");
          exit(0);
      case  0:
          pusac3();
          exit(0);
      default:
          break;
  }


  for(i=0; i<=4; i++) {
        wait(NULL);
  }
  semRemove();

  return(0);
}
