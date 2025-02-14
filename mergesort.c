#include <stdio.h>
#include <stdlib.h>
#include "sorting.h"

int main(void) {

  FILE *f = stdin;
  unsigned long n = 0;
  fscanf(f, "%lu", &n);

  unsigned long *v = malloc(sizeof(*v) * n);
  for (unsigned long i = 0; i < n; i++) {
    fscanf(f, "%lu", &v[i]);
  }

  mergesort(v, n);

  /*
  for (unsigned long i = 0; i < n; i++) {
    printf("%lu ", v[i]);
  }
  printf("\n");
  */

  if (testa(v, n))
    printf("erro...\n");
  else
    printf("vetor ordenado!\n");

  free(v);

  return 0;
}
