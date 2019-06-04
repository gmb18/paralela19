#include <stdio.h>
#include <stdlib.h>

static void merge(unsigned long *v, unsigned long *w, unsigned long i, unsigned long  m, unsigned long f) {
  // dado vetor v, sendo v[i..m] e  v[m+1..f] já ordenados, coloca os 
  // elementos dos dois ordenadamente em v[i..f]
  unsigned long z; 
  unsigned long iv = i;
  unsigned long ic = m+1;

  for (z = i; z <= f; z++) {
    w[z] = v[z];
  }
  z = i;

  while (iv <= m && ic <= f) {
    // invariante: v[i..z] possui valores de v[iv..m] e v[ic..f] ordenados
    if (w[iv] < w[ic]) {
      v[z++] = w[iv++];
    } else {
      v[z++] = w[ic++];
    }
  }

  while (iv <= m) {
    v[z++] = w[iv++];
  }

  while (ic <= f) {
    v[z++] = w[ic++];
  }
}

static void sort(unsigned long *v, unsigned long *w, unsigned long i, unsigned long f) {
  if (i >= f)
    return;

  unsigned long m = (i + f) / 2;

  sort(v, w, i, m);
  sort(v, w, m+1, f);

  if (v[m] <= v[m+1])
    // então v[i...f] já está ordenado
    return;

  merge(v, w, i, m, f);
}

static void mergesort(unsigned long *v, unsigned long n) {
  unsigned long *w = malloc(sizeof(*w) * n);
  sort(v, w, 0, n-1);
  free(w);
}

int main() {

  FILE *f = stdin;
  unsigned long n = 0;
  fscanf(f, "%lu", &n);

  unsigned long *v = malloc(sizeof(*v) * n);
  for (unsigned long i = 0; i < n; i++) {
    fscanf(f, "%lu", &v[i]);
  }

  mergesort(v, n);

  for (unsigned long i = 0; i < n; i++) {
    printf("%lu ", v[i]);
  }
  printf("\n");

  free(v);

  return 0;
}
