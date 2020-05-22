#include <stdio.h>
#include <malloc.h>
#include <unistd.h>

// prvi dio je isti kao u simulacija.c

int t,p;

typedef struct Dretva {
  int id;
  int preostaloVrijeme;
  int prioritet;
  int tip; // dodan samo tip dretve
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
dretva *novaDretva;

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
      printf("  %d/%d/%d/%d", P[i]->id, P[i]->preostaloVrijeme, P[i]->prioritet, P[i]->tip);
    } else {
      printf("  -/-/-/-" );
    }
    printf("\n");
  }
}


void rr(void) {
	int i;
	for (i = 0; i < p-1; ++i) {
    P[i]=P[i+1];
	}
	P[p-1]=NULL;
	for (i=p-1; i>0; --i) {
		if (P[i-1]->prioritet < novaDretva->prioritet) {
			P[i] = P[i-1];
		}
		else {
			break;
		}
	}
	P[i]=novaDretva;
}

void rasporedi(void) {
	int i=0;
	if (P[i] == NULL) {
		P[i]=novaDretva;
		return;
	}
	else {
		for (i = p-1; i>0; --i) {
			if (P[i-1]->prioritet < novaDretva->prioritet) {
				P[i]=P[i-1];
			}
			else {
				break;
			}
		}
		P[i]=novaDretva;
	}
}

int main (void) {

  int j;
  for (j = 0; j < MAX_DRETVI; ++j) {
    P[j] = NULL;
  }

  novaDretva = NULL;
  t = 0;
  p = 0;
  int k;

	while (1) {
		if (t==0)
			ispis_stanja(1);
		else {
			ispis_stanja(0);
		}
		for (k = 0; k < DRETVI; ++k) {
			if (p < MAX_DRETVI && nove[k][0] == t) {
				novaDretva = (dretva*) malloc (sizeof(dretva));
				novaDretva->id = nove[k][1];
				novaDretva->preostaloVrijeme = nove[k][2];
				novaDretva->prioritet = nove[k][3];
				novaDretva->tip = nove[k][4];
        int a = p;
        p++;

				rasporedi();
				printf ("%3d nova dretva id=%d -  p=%d -  prio=%d - rasp=%d\n", t, P[a]->id, P[a]->preostaloVrijeme, P[a]->prioritet, P[a]->tip);
				ispis_stanja(0);
			}
		}

		if (P[0] != NULL) {
			P[0]->preostaloVrijeme--;
			if (P[0]->preostaloVrijeme == 0) {
				printf ("Dretva %d zavrsila\n", P[0]->id);
				for (k=0; k < p-1; ++k) {
					P[k] = P[k+1];
				}
				P[p-1]=NULL;
				p--;
			}
			else if (P[0]->tip) {
				novaDretva = P[0];
				rr();
			}
		}
		sleep(1);
    t++;
	}
	return 0;
}
