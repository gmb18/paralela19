#include <stdio.h>
#include <stdlib.h>

static void troca(unsigned long *v, unsigned long a, unsigned long b) {
  unsigned long temp = v[a];
  v[a] = v[b];
  v[b] = temp;
}

static unsigned long partition(unsigned long *v, unsigned long low, unsigned long high) {
  // pega último valor como pivô, coloca ele no lugar certo e os outros também
  unsigned long pivot = v[high];
  unsigned long i = low - 1; // index do menor elemento

  for (unsigned long j = low; j <= high-1; j++) {
    if (v[j] <= pivot) {
      i++;
      troca(v, i, j);
    }
  }
  troca(v, i+1, high);

  return i+1;
}

static void quicksort(unsigned long *v, unsigned long low, unsigned long high) {
  if (low < high) {
    // pi particiona o vetor, v[pi] está no lugar certo
    unsigned long pi = partition(v, low, high);
    // agora ordena elementos antes e depois da partição
    quicksort(v, low, pi-1);
    quicksort(v, pi+1, high);
  }
}

int main() {

  FILE *f = stdin;
  unsigned long n = 0;
  fscanf(f, "%lu", &n);

  unsigned long *v = malloc(sizeof(*v) * n);
  for (unsigned long i = 0; i < n; i++) {
    fscanf(f, "%lu", &v[i]);
  }

  quicksort(v, 0, n-1);

  for (unsigned long i = 0; i < n; i++) {
    printf("%lu ", v[i]);
  }
  printf("\n");

  free(v);

  return 0;
}
