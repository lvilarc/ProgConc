#include <stdio.h>
#include <stdlib.h> 
#include <sys/time.h>


/* Multiplicação entre 2 matrizes A e B */
float *multm(float *a, int la, int ca, float *b, int lb, int cb) {
   if(ca!=lb) {
      printf("ERROR: 'Cannot multiply'\n\n\n");
      return NULL;
   }
   float *c;
   int i, j, k;
   c = malloc(la*cb*sizeof(float));
   if(c==NULL) {printf("ERROR");}

   for(i = 0; i < la; i++) {
      for(j = 0; j < ca; j++) {
         c[i*la+j] = 0;
         for(k = 0; k < ca; k++) {
               c[i*la+j] += a[i*la+k]*b[k*cb+j];
         }
      }
   }
return c;
}


int main(int argc, char*argv[]) {
   struct timeval t0, t1, t2, t3;
   gettimeofday(&t0, 0);
   // GET_TIME(start);

   float *matriza, *matrizb, *matrizc; //matriza que será carregada do arquivo
   int la, ca, lb, cb; //dimensoes da matriza
   long long int tama, tamb, tamc; //qtde de elementos na matriza
   FILE * descritorArquivo; //descritor do arquivo de entrada
   size_t ret; //retorno da funcao de leitura no arquivo de entrada
   
   //recebe os argumentos de entrada
   if(argc < 2) {
      fprintf(stderr, "Digite: %s <arquivo entrada matriz A> <arquivo de entrada matriz B> <arquivo exit C>\n", argv[0]);
      return 1;
   }
   
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

   //imprime a matriza na saida padrao
   // printf("Matriz A:\n");
   // for(int i=0; i<la; i++) { 
   //    for(int j=0; j<ca; j++)
   //      fprintf(stdout, "%.6f ", matriza[i*ca+j]);
   //    fprintf(stdout, "\n");
   // }
   // printf("\n");

   fclose(descritorArquivo);


   /////////////////////

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

   //imprime a matrizb na saida padrao
   // printf("Matriz B:\n");
   // for(int i=0; i<lb; i++) { 
   //    for(int j=0; j<cb; j++)
   //      fprintf(stdout, "%.6f ", matrizb[i*cb+j]);
   //    fprintf(stdout, "\n");
   // }
   // printf("\n");

   //finaliza o uso das variaveis
   fclose(descritorArquivo);

   tamc = la*cb;

   matrizc = malloc(la*cb*sizeof(float));

   gettimeofday(&t1, 0);

   matrizc = multm(matriza, la, ca, matrizb, lb, cb);

   gettimeofday(&t2, 0);

   // printf("Matriz C:\n");
   // for(int i=0; i<la; i++) { 
   //    for(int j=0; j<cb; j++)
   //      fprintf(stdout, "%.6f ", matrizc[i*la+j]);
   //    fprintf(stdout, "\n");
   // }
   // printf("\n");

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
   
   return 0;
}



