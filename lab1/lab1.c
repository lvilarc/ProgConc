#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS  2 

float vetor[10000];

typedef struct {
   int tam, idThread, nThreads;
} t_Args;

void *Increment (void *arg) {
  t_Args *args = (t_Args *) arg;
  for (int i = args->idThread; i < args->tam; i+=args->nThreads) {
      vetor[i] *= 1.1;
  }
  free(arg); //aqui pode liberar a alocacao feita na main
  pthread_exit(NULL);
}

int main() {
  t_Args *arg;
  for(int i = 0; i < 10000; i++) {
      vetor[i] = i;
      //printf("%f ", vetor[i]);
  }
  for(int i = 0; i < 5; i++) {
      printf("%f ", vetor[i]);
  }
  printf("\n");
  pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
  int thread; //variavel auxiliar
  

  for(thread=0; thread<NTHREADS; thread++) {
    arg = malloc(sizeof(t_Args));
    if (arg == NULL) {
      printf("--ERRO: malloc()\n"); exit(-1);
    }
    arg->idThread = thread; 
    arg->nThreads = NTHREADS;
    arg->tam = 10000;
    if (pthread_create(&tid_sistema[thread], NULL, Increment, (void*) arg)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }

  //--espera todas as threads terminarem
  for (thread=0; thread<NTHREADS; thread++) {
    if (pthread_join(tid_sistema[thread], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    }
  }
  for(int i = 0; i < 5; i++) {
      printf("%f ", vetor[i]);
  }
  pthread_exit(NULL);
}