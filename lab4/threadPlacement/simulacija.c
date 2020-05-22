// radjeno po predlosku prof. Jelenkovica

#include <stdio.h>
#include <malloc.h>
#include <unistd.h>

int t;

typedef struct Dretva {
  int id;
  int preostaloVrijeme;
  int prioritet; // NEPOTREBAN JER RADIMO FIFO
} dretva;

#define MAX_DRETVI 5
#define DRETVI 6

int nove[DRETVI][5] = {
  { 1,  3, 5, 3, 1 },
	{ 3,  5, 6, 5, 1 },
	{ 7,  2, 3, 5, 0 },
	{ 12, 1, 5, 3, 0 },
	{ 20, 6, 3, 6, 1 },
	{ 20, 7, 4, 7, 1 },
};

dretva *P[MAX_DRETVI];
//dretva *nova;

void ispis_stanja(int ispisi_zaglavlje) {
  int i;
  if (ispisi_zaglavlje == 1) {
    printf("  t    AKT");
    for (i = 1; i < MAX_DRETVI; ++i) {
      printf("     PR%d", i);
    }
    printf("\n");
  }
  printf("%3d ", t);

  for (i = 0; i < MAX_DRETVI; i++) {
    if (P[i] != NULL) {
      printf("  %d/%d ", P[i]->id, P[i]->preostaloVrijeme);
    } else {
      printf("  -/-/- " );
    }
    printf("\n");
  }
}

// radjen po pseudokodu uputa za labos prof. Jelenkovica
int main(void) {

  int j;
  for (j = 0; j < MAX_DRETVI; ++j) {
    P[j] = NULL;
  }

  dretva *novaDretva = NULL;
  t = 0;
  int p = 0;
  int k;

  while (1) {
    if (t==0) {
      ispis_stanja(1); // na pocetku samo zaglavlje
    } else {
      ispis_stanja(0);
    }

    for (k = 0; k < DRETVI; ++k) {
      if (p < MAX_DRETVI && nove[k][0] == t) {
        novaDretva = (dretva*)malloc (sizeof(dretva));
        novaDretva->id = nove[k][1];
        novaDretva->preostaloVrijeme = nove[k][2];
        P[p] = novaDretva; // dodavanje u pripravne
        printf ("%3d -- nova dretva id=%d, p=%d\n", t, P[p]->id, P[p]->preostaloVrijeme);
        p++;
      }
    }
    if (P[0] != NULL) {
      P[0]->preostaloVrijeme--;
      if (P[0]->preostaloVrijeme == 0) {
        printf ("Dretva %d zavrsila\n", P[0]->id);
        for (k = 0; k < p - 1; ++k) {
          P[k] = P[k+1];
        }
        P[p-1] = NULL;
        p--;
      }
    }
    sleep(1);
    t++;
  }
  return 0;
}
