#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sorting.h"

int main(void) {

  FILE *f = stdin;
  unsigned long n = 0;
  fscanf(f, "%lu", &n);

  unsigned long *v = malloc(sizeof(*v) * n);
  for (unsigned long i = 0; i < n; i++) {
    fscanf(f, "%lu", &v[i]);
  }

  clock_t t1 = clock();
  quicksort(v, 0, n-1);
  clock_t t2 = clock();
  double time_taken = (double)(t2 - t1) / CLOCKS_PER_SEC;
  printf("Vetor de tamanho %lu ordenado em %3.3f\n", n, time_taken);

  /*
  for (unsigned long i = 0; i < n; i++) {
    printf("%lu ", v[i]);
  }
  printf("\n");
  */
  
  /*
  if (testa(v, n)) {
    printf("erro...\n");
    for (unsigned long i = 0; i < n; i++) {
      printf("%lu ", v[i]);
    }
    printf("\n");
  } else
    printf("vetor ordenado!\n");
    */

  free(v);

  return 0;
}
