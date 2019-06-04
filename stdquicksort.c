#include <stdio.h>
#include <stdlib.h>

static int f_compara(const void *a, const void *b) {
  return (*(unsigned long *)a - *(unsigned long *)b);
}

int main() {

  FILE *f = stdin;
  unsigned long n = 0;
  fscanf(f, "%lu", &n);

  unsigned long *v = malloc(sizeof(*v) * n);
  for (unsigned long i = 0; i < n; i++) {
    fscanf(f, "%lu", &v[i]);
  }

  qsort(v, n, sizeof(*v), f_compara);

  for (unsigned long i = 0; i < n; i++) {
    printf("%lu ", v[i]);
  }
  printf("\n");

  free(v);

  return 0;
}
