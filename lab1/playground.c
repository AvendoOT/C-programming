#define _XOPEN_SOURCE 500
#include <signal.h>
#include <stdio.h>
#include "time.h"
#include "unistd.h"


void obradi(int sig) {
  printf("\nPocetak obrade ctrl+c");
  sleep(5);
  printf("\nZavrsetak obrade\n");
}

int main(void) {
    printf("Signal se obradjuje ctrl +c\n");
    sigset(SIGINT, obradi);
    int i = 0;
    for (i; i < 10; i++) {
      sleep(1);
    }
  return 0;
}
