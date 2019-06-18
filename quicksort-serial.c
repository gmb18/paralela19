#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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


int testa(unsigned long *v, unsigned long n) {
  // retorna 0 se vetor v está ordenado
  for (unsigned long i = 1; i < n; i++) {
    if (v[i-1] > v[i]) {
      return 1;
    }
  }
  return 0;
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
  printf("Vetor de tamanho %lu ordenado em %3.3f com quicksort serial\n", n, time_taken);

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
