#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>


pthread_mutex_t monitor;

// ako ima partibrejera 1, 0 ako nema
bool imaLiPartibrejkera;
int brojLjudi;
int ljudiSoba;
int ljudiDretva;

void soba(int noviLjudi) {

    pthread_mutex_lock(&monitor);

    // ako nema partibrejkera u sobi ljudi slobodno ulaze
    if(!imaLiPartibrejkera) {
        ljudiSoba += noviLjudi;
    }
    pthread_mutex_unlock(&monitor);
}


void* student(void *x) {
    // [100, 500]
    usleep((rand()%(500-100+1)+100)*1000);
    int i = 0;
    // za vise od troje ljudi provjeravamo partibrejkera
    while (i<3) {
      // ako u sobi nema partibrejkera student ulazi
        if(!imaLiPartibrejkera && ljudiDretva >= 0) {
            soba(1);
            ljudiDretva++;
            printf("Student %d je usao u sobu\n", ljudiDretva);
        }

        // [100, 1000]
        usleep((rand()%(1000 - 100 +1)+100)*1000);
        // ukoliko ima ljudi na cekanju, student izlazi
        if(ljudiDretva >= 1) {
            soba(-1);
            printf("Student %d je izasao iz sobe\n", ljudiDretva--);
        }
        // [100, 1000]
        usleep((rand()%(1000-100+1)+100)*1000);
        i++;
    }

}

void *partibrejker() {

    while(brojLjudi > 1) {
        // [100, 1000]
        usleep((rand()%(1000-100+1)+100)*1000);
        // ako je u sobi vise od dvoje ljudi partibrejker ulazi
        if(ljudiSoba >= 3 && !imaLiPartibrejkera) {
            soba(1);
            imaLiPartibrejkera = true;
            printf("Partibrejker je usao u sobu\n");

        }
        // partibrejker izlazi kad vise nema ljudi u sobi
        else if(imaLiPartibrejkera && ljudiDretva == 0) {
            soba(-1);
            imaLiPartibrejkera = false;
            printf("Partibrejker je izasao iz sobe\n");
        }
    }
}

int main(int argc, char* argv[])
{
    if(argc == 1)
    {
        printf("Pogresan broj argumenata");
        return 0;
    }

    int n = atoi(argv[1]);
    pthread_mutex_init(&monitor, NULL);

    pthread_t thr[n+1];
    brojLjudi = 0;
    imaLiPartibrejkera = false;
    ljudiSoba = 0;
    ljudiDretva = 0;

    for(int i = 0; i<n; i++)
    {
        if(pthread_create(&thr[i], NULL, student, &i)){
            printf("Dretva se ne moze stvoriti");
            exit(1);
        }
        brojLjudi++;
    }
    if(pthread_create(&thr[n], NULL, partibrejker, NULL)){
        printf("Dretva se ne moze stvoriti");
        exit(1);
    }
    for(int i = 0; i<n+1; i++){
        if(pthread_join(thr[i], NULL)) {
            fprintf(stderr, "Dretva se ne moze spojiti\n");
            return 1;
        }
        brojLjudi--;
    }

    pthread_mutex_destroy(&monitor);

    return 0;
}
