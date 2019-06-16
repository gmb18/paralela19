#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "sorting.h"

static void paramistosort(unsigned long *v, unsigned long *w, unsigned long i, unsigned long f, unsigned long parte) {
  //printf("  paramisto sort thread %d i: %lu, f: %lu\n", omp_get_thread_num(), i, f);
  if (i >= f)
    return;

  if ((f - i) <= parte) {
//#pragma omp task
    ////printf("        thread %d passando pro quicksort\n", omp_get_thread_num());
    quicksort(v, i, f);
    return;
  }
  unsigned long m = (i + f) / 2;

#pragma omp task
  paramistosort(v, w, i, m, parte);
#pragma omp task
  paramistosort(v, w, m+1, f, parte);
#pragma omp taskwait

  //if (v[m] <= v[m+1])
    // então v[i...f] já está ordenado
    //return;

  //printf("    thread %d passando pro merge\n", omp_get_thread_num());
  merge(v, w, i, m, f);
}

static void paramisto(unsigned long *v, unsigned long n) {
  unsigned long *w = malloc(sizeof(*w) * n);
  //printf("começando parallel mergesort misto\n");
#pragma omp parallel num_threads(2)
//#pragma omp parallel 
#pragma omp single
  {
  int p = omp_get_num_threads();
  //printf("total de threads: %d\n", p);
  paramistosort(v, w, 0, n-1, n/(unsigned long)p);
  free(w);
  }
}

int main(int argc, char **argv) {

  FILE *f = stdin;
  unsigned long n = 0;
  if (!fscanf(f, "%lu", &n))
    printf("erro fscanf\n");

  unsigned long *v = malloc(sizeof(*v) * n);
  for (unsigned long i = 0; i < n; i++) {
    if (!fscanf(f, "%lu", &v[i]))
      printf("erro fscanf\n");
  }

  if (argc == 2) 
    omp_set_num_threads(atoi(argv[1]));
  clock_t t1 = clock();
  paramisto(v, n);

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
  clock_t t2 = clock();
  double time_taken = (double)(t2 - t1) / CLOCKS_PER_SEC;
  printf("Vetor de tamanho %lu ordenado em %3.3f\n", n, time_taken);

  free(v);

  return 0;
}
