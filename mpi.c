// algoritmo tirade do site:
// http://penguin.ewu.edu/~trolfe/ParallelMerge/ParallelMerge.html
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

#define  INIT  1        // mensagem passando tamanho e altura
#define  DATA  2        // mensagem passando vetor a ser ordenado
#define  ANSW  3        // mensagem retornando vetor já ordenado
#define  FINI  4        // manda permissão pra terminar

int testa(long *v, long n);
void troca(long *v, long a, long b);
void paramerge(long *v, long tamanho, long altura);
void quicksort(long *v, long low, long high);
long partition(long *v, long low, long high);

void paramerge(long *v, long tamanho, long minhaAltura) {
  int rc, meuRank, nProcessos;
  long pai, proximo, filhoDireita = 0;

  rc = MPI_Comm_rank(MPI_COMM_WORLD, &meuRank);
  rc = MPI_Comm_size(MPI_COMM_WORLD, &nProcessos);

  pai = meuRank & ~(1 << minhaAltura);
  proximo = minhaAltura - 1;
  if (proximo >= 0) {
    filhoDireita = meuRank | (1 << proximo);
  }
  
  if (minhaAltura > 0) {
    if (filhoDireita >= nProcessos) {
      // sem filho à direita mais, pode descer um nível
      paramerge(v, tamanho, proximo);

    } else {
      long tam_esquerda = tamanho / 2;
      long tam_direita = tamanho - tam_esquerda;
      long *v_esquerda = malloc(sizeof(*v_esquerda) * (unsigned)tam_esquerda);
      long *v_direita = malloc(sizeof(*v_direita) * (unsigned)tam_direita);
      long iV[2];
      long i, j, k; // usados no merge
      MPI_Status status;

      memcpy(v_esquerda, v, sizeof(*v_esquerda) * (unsigned)tam_esquerda);
      memcpy(v_direita, v+tam_esquerda, sizeof(*v_direita) * (unsigned)tam_direita);
      iV[0] = tam_direita;
      iV[1] = proximo;
      rc = MPI_Send(iV, 2, MPI_LONG, (int)filhoDireita, INIT, MPI_COMM_WORLD);
      rc = MPI_Send(v_direita, (int)tam_direita, MPI_LONG, (int)filhoDireita, DATA, MPI_COMM_WORLD);

      paramerge(v_esquerda, tam_esquerda, proximo);

      rc = MPI_Recv(v_direita, (int)tam_direita, MPI_LONG, (int)filhoDireita, ANSW, MPI_COMM_WORLD, &status);

      ///////////////MERGE
      i = j = k = 0;
      while (i < tam_esquerda && j < tam_direita) {
        if (v_esquerda[i] > v_direita[j]) {
          v[k++] = v_direita[j++];
        } else {
          v[k++] = v_esquerda[i++];
        }
      }
      while (i < tam_esquerda) {
        v[k++] = v_esquerda[i++];
      }
      while (j < tam_direita) {
        v[k++] = v_direita[j++];
      }
      ////////////////////////////////
    }
  } else {
    // altura = 0  ->  nodo folha, ordenamento sequencial
    quicksort(v, 0, tamanho-1);
  }

  if (pai != meuRank) {
    // obs: se pai != meuRank, então esse é um nodo da direita
    // e o vetor ordenado precisa ser devolvido por mensagem pro pai
    // (nos outros casos a 'comunicação' é automática, pois o vetor
    // é passado como argumento)
    rc = MPI_Send(v, (int)tamanho, MPI_LONG, (int)pai, ANSW, MPI_COMM_WORLD);
  }
}

int testa(long *v, long n) {
  // retorna 0 se vetor v está ordenado
  for (long i = 1; i < n; i++) {
    if (v[i-1] > v[i]) {
      return 1;
    }
  }
  return 0;
}

void troca(long *v, long a, long b) {
  long temp = v[a];
  v[a] = v[b];
  v[b] = temp;
}

void quicksort(long *v, long low, long high) {
  if (low < high) {
    // pi particiona o vetor, v[pi] está no lugar certo
    long pi = partition(v, low, high);
    // agora ordena elementos antes e depois da partição
    quicksort(v, low, pi ? pi-1 : 0);
    quicksort(v, pi+1, high);
  }
}

long partition(long *v, long low, long high) {
  // pega último valor como pivô, coloca ele no lugar certo e os outros também
  long pivot = v[high];
  long i = low - 1; // index do menor elemento
  //long i = low > 0 ? low - 1 : 0;

  for (long j = low; j <= high-1; j++) {
    if (v[j] <= pivot) {
      i++;
      troca(v, i, j);
    }
  }
  troca(v, i+1, high);

  return i+1;
}

int main(int argc, char **argv) {

  int meuRank, nProcessos;
  int rc;
  long *v, tamanho;
  double start, finnish;
  clock_t t1 = 0;;

  rc = MPI_Init(&argc, &argv);
  if (rc < 0) {
    printf("failed to start MPI...");
    exit(-1);
  }
  rc = MPI_Comm_rank(MPI_COMM_WORLD, &meuRank);
  rc = MPI_Comm_size(MPI_COMM_WORLD, &nProcessos);

  if (meuRank == 0) { 
    //host raiz
    long alturaRaiz = 0;
    int contNodos = 1;
    while (contNodos < nProcessos) {
      contNodos+= contNodos;
      alturaRaiz++;
    }

    FILE *f = stdin;
    if (!fscanf(f, "%lu", &tamanho)) {
      printf("erro fscanf\n");
    }
    v = malloc(sizeof(*v) * (unsigned)tamanho);
    for (long i = 0; i < tamanho; i++) {
      if (!fscanf(f, "%lu", &v[i]))
        printf("erro fscanf\n");
    }

    start = MPI_Wtime();
    t1 = clock();
    paramerge(v, tamanho, alturaRaiz);
  } else { 
    //outros nós
    long iV[2], //mensagem a ser passada, [0] = tamanho e [1] = altura
        altura; //tirado de iV[]
    MPI_Status status; //precisa pra receber msg
    rc = MPI_Recv(iV, 2, MPI_LONG, MPI_ANY_SOURCE, INIT, MPI_COMM_WORLD, &status);
    tamanho = iV[0]; //tamanho isolado
    altura = iV[1];
    v = malloc(sizeof(*v) * (unsigned)tamanho);
    rc = MPI_Recv(v, (int)tamanho, MPI_LONG, MPI_ANY_SOURCE, DATA, MPI_COMM_WORLD, &status);
    paramerge(v, tamanho, altura);
    MPI_Finalize();
    //sai do processo
    return 0;
  }

  MPI_Finalize();
  finnish = MPI_Wtime();
  //if (!testa(v, tamanho)) {
    //printf("Vetor de tamanho %lu ordenado em %3.3f usando %d processos\n", tamanho, finnish - start, nProcessos);
  //}else{
    //printf("erro...\n");
  //}
  clock_t t2 = clock();
  double time_taken = (double)(t2 - t1) / CLOCKS_PER_SEC;
  printf("Vetor de tamanho %lu ordenado em %3.3f com %d processos (mpi)\n", tamanho, time_taken, nProcessos);

  return 0;
}
