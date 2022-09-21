#include <stdio.h>
#include <stdlib.h> 
#include <sys/time.h>
#include <pthread.h>

/* Matriz C */
float *matrizc;

/* Argumentos para as threads */
typedef struct {
   int idThread, nThreads;
   float *a, *b, *c;
   int la, ca, lb, cb;
} t_Args;

/* Tarefa Concorrente */
void *Tarefa (void *arg) {
   t_Args *args = (t_Args *) arg;

   if(args->ca!=args->lb) {
      printf("ERROR: 'Cannot multiply'\n");
      exit(0);
   }
   int i, j, k;
   for(i = args->idThread; i < args->la; i += args->nThreads) {
      for(j = 0; j < args->ca; j++) {
         args->c[i*args->la+j] = 0;
         for(k = 0; k < args->ca; k++) {
               args->c[i*args->la+j] += args->a[i*args->la+k]*args->b[k*args->cb+j];
         }
      }
   }
   pthread_exit(NULL);
}

int main(int argc, char*argv[]) {
   struct timeval t0, t1, t2, t3;
   gettimeofday(&t0, 0);

   int nThreads;

   float *matriza, *matrizb; //matrizes de entrada
   int la, ca, lb, cb; //linhas e colunas das matrizes de entrada
   long long int tama, tamb, tamc; // quantidade de elementos das 3 matrizes 
   FILE * descritorArquivo; //descritor do arquivo de entrada
   size_t ret; //retorno da funcao de leitura no arquivo de entrada
   
   //recebe os argumentos de entrada
   if(argc < 5) {
      fprintf(stderr, "Digite: %s <arquivo entrada matriz A> <arquivo de entrada matriz B> <arquivo exit C> <nThreads>\n", argv[0]);
      return 1;
   }

   nThreads = atoi(argv[4]);
   
   //abre o arquivo para leitura binaria
   descritorArquivo = fopen(argv[1], "rb");
   if(!descritorArquivo) {
      fprintf(stderr, "Erro de abertura do arquivo\n");
      return 2;
   }

   //le as dimensoes da matriza
   ret = fread(&la, sizeof(int), 1, descritorArquivo);
   if(!ret) {
      fprintf(stderr, "Erro de leitura das dimensoes da matriza arquivo \n");
      return 3;
   }
   ret = fread(&ca, sizeof(int), 1, descritorArquivo);
   if(!ret) {
      fprintf(stderr, "Erro de leitura das dimensoes da matriza arquivo \n");
      return 3;
   }
   tama = la * ca; //calcula a qtde de elementos da matriza

   //aloca memoria para a matriza
   matriza = (float*) malloc(sizeof(float) * tama);
   if(!matriza) {
      fprintf(stderr, "Erro de alocao da memoria da matriza\n");
      return 3;
   }

   //carrega a matriza de elementos do tipo float do arquivo
   ret = fread(matriza, sizeof(float), tama, descritorArquivo);
   if(ret < tama) {
      fprintf(stderr, "Erro de leitura dos elementos da matriza\n");
      return 4;
   }


   fclose(descritorArquivo);


   ////////////////
   ////////////////

   //abre o arquivo para leitura binaria
   descritorArquivo = fopen(argv[2], "rb");
   if(!descritorArquivo) {
      fprintf(stderr, "Erro de abertura do arquivo\n");
      return 2;
   }

   //le as dimensoes da matrizb
   ret = fread(&lb, sizeof(int), 1, descritorArquivo);
   if(!ret) {
      fprintf(stderr, "Erro de leitura das dimensoes da matrizb arquivo \n");
      return 3;
   }
   ret = fread(&cb, sizeof(int), 1, descritorArquivo);
   if(!ret) {
      fprintf(stderr, "Erro de leitura das dimensoes da matrizb arquivo \n");
      return 3;
   }
   tamb = lb * cb; //calcula a qtde de elementos da matrizb

   //aloca memoria para a matrizb
   matrizb = (float*) malloc(sizeof(float) * tamb);
   if(!matrizb) {
      fprintf(stderr, "Erro de alocao da memoria da matrizb\n");
      return 3;
   }

   //carrega a matrizb de elementos do tipo float do arquivo
   ret = fread(matrizb, sizeof(float), tamb, descritorArquivo);
   if(ret < tamb) {
      fprintf(stderr, "Erro de leitura dos elementos da matrizb\n");
      return 4;
   }

   //finaliza o uso das variaveis
   fclose(descritorArquivo);

   tamc = la*cb;

   matrizc = malloc(la*cb*sizeof(float));
   if (matrizc == NULL) {
         printf("--ERRO: malloc()\n"); exit(-1);
      }

   gettimeofday(&t1, 0);

   ////////////////
   ///////////////

   pthread_t tid_sistema[nThreads]; //identificadores das threads no sistema
   int thread; //variavel auxiliar
   t_Args *arg;

   for(thread=0; thread<nThreads; thread++) {
      arg = malloc(sizeof(t_Args));
      if (arg == NULL) {
         printf("--ERRO: malloc()\n"); exit(-1);
      }
      arg->idThread = thread; 
      arg->nThreads = nThreads;
      arg->a = matriza;
      arg->b = matrizb;
      arg->c = matrizc;
      arg->la = la;
      arg->ca = ca;
      arg->lb = lb;
      arg->cb = cb;
      if (pthread_create(&tid_sistema[thread], NULL, Tarefa, (void*) arg)) {
         printf("--ERRO: pthread_create()\n"); exit(-1);
      }
   }

      //--espera todas as threads terminarem
   for (thread=0; thread<nThreads; thread++) {
      if (pthread_join(tid_sistema[thread], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
      }
   }

   free(arg);

   ///////////////
   //////////////

   gettimeofday(&t2, 0);

   //////////////////////
   ////////////////////

   //escreve a matriz no arquivo
   //abre o arquivo para escrita binaria
   descritorArquivo = fopen(argv[3], "wb");
   if(!descritorArquivo) {
      fprintf(stderr, "Erro de abertura do arquivo\n");
      return 3;
   }
   //escreve numero de linhas e de colunas
   ret = fwrite(&la, sizeof(int), 1, descritorArquivo);
   //escreve os elementos da matriz
   ret = fwrite(&cb, sizeof(int), 1, descritorArquivo);
   ret = fwrite(matrizc, sizeof(float), tamc, descritorArquivo);
   if(ret < tamc) {
      fprintf(stderr, "Erro de escrita no  arquivo\n");
      return 4;
   }

   //finaliza o uso das variaveis
   fclose(descritorArquivo);



   // Calculo dos tempos
   gettimeofday(&t3, 0);

   long seconds = t1.tv_sec - t0.tv_sec;
   long microseconds = t1.tv_usec - t0.tv_usec;
   double elapsed = seconds + microseconds*1e-6;
   printf("i: %.6f seconds.\n", elapsed);

   seconds = t2.tv_sec - t1.tv_sec;
   microseconds = t2.tv_usec - t1.tv_usec;
   elapsed = seconds + microseconds*1e-6;
   printf("p: %.6f seconds.\n", elapsed);

   seconds = t3.tv_sec - t2.tv_sec;
   microseconds = t3.tv_usec - t2.tv_usec;
   elapsed = seconds + microseconds*1e-6;
   printf("f: %.6f seconds.\n", elapsed);

   pthread_exit(NULL);
}



