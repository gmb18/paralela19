#include <stdlib.h>
#include "sorting.h"

void merge(unsigned long *v, unsigned long *w, unsigned long i, unsigned long  m, unsigned long f) {
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

void mergesort(unsigned long *v, unsigned long n) {
  unsigned long *w = malloc(sizeof(*w) * n);
  sort(v, w, 0, n-1);
  free(w);
}


static void troca(unsigned long *v, unsigned long a, unsigned long b) {
  unsigned long temp = v[a];
  v[a] = v[b];
  v[b] = temp;
}

static unsigned long partition(unsigned long *v, unsigned long low, unsigned long high) {
  // pega último valor como pivô, coloca ele no lugar certo e os outros também
  unsigned long pivot = v[high];
  unsigned long i = low - 1; // index do menor elemento
  //unsigned long i = low > 0 ? low - 1 : 0;

  for (unsigned long j = low; j <= high-1; j++) {
    if (v[j] <= pivot) {
      i++;
      troca(v, i, j);
    }
  }
  troca(v, i+1, high);

  return i+1;
}

static void mistosort(unsigned long *v, unsigned long *w, unsigned long i, unsigned long f, unsigned long parte) {
  if (i >= f)
    return;

  if ((f - i) <= parte) {
    quicksort(v, i, f);
    return;
  }
  unsigned long m = (i + f) / 2;

  mistosort(v, w, i, m, parte);
  mistosort(v, w, m+1, f, parte);

  if (v[m] <= v[m+1])
    // então v[i...f] já está ordenado
    return;

  merge(v, w, i, m, f);
}

void quicksort(unsigned long *v, unsigned long low, unsigned long high) {
  if (low < high) {
    // pi particiona o vetor, v[pi] está no lugar certo
    unsigned long pi = partition(v, low, high);
    // agora ordena elementos antes e depois da partição
    quicksort(v, low, pi ? pi-1 : 0);
    quicksort(v, pi+1, high);
  }
}

void misto(unsigned long *v, unsigned long n, int p) {
  unsigned long *w = malloc(sizeof(*w) * n);
  mistosort(v, w, 0, n-1, n/(unsigned long)p);
  free(w);
}

int testa(unsigned long *v, unsigned long n) {
  // retorna 0 se vetor v está ordenado
  for (unsigned long i = 1; i < n; i++) {
    if (v[i-1] > v[i]) {
      return 1;
    }
  }
  return 0;
}
