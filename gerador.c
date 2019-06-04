#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {

  if (argc != 2) {
    printf("uso: %s n\n", argv[0]);
    exit(1);
  }
  unsigned long n = atoi(argv[1]);
  
  srand(time(NULL));
  printf("%lu\n", n);
  for (unsigned long i = 0; i < n; i++) {
    printf("%lu\n", random() % n);
  }

  return 0;
}
