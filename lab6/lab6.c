/* LAB 6                                     */
/* Aluno: Lucas Vilar Carneiro DRE 119108874 */

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define QT1 2 // numero de threads TIPO 1
#define QT2 2 // numero de threads TIPO 2
#define QT3 2 // numero de threads TIPO 3

// variavel global
int global = 0;

// variaveis do problema
int leit = 0; // contador de threads lendo
int escr = 0; // contador de threads escrevendo

// variaveis para sincronizacao
pthread_mutex_t mutex;
pthread_cond_t cond_leit, cond_escr;

// entrada leitura
void InicLeit (int id, int tipo) {
    pthread_mutex_lock(&mutex);
    printf("Thread[%d] de TIPO %d quer ler\n", id, tipo);
    while(escr > 0) {
        printf("Thread[%d] de TIPO %d bloqueou\n", id, tipo);
        pthread_cond_wait(&cond_leit, &mutex);
        printf("Thread[%d] de TIPO %d desbloqueou\n", id, tipo);
    }
    leit++;
    pthread_mutex_unlock(&mutex);
}

// saida leitura
void FimLeit(int id, int tipo) {
    pthread_mutex_lock(&mutex);
    printf("Thread[%d] de TIPO %d terminou de ler\n", id, tipo);
    leit--;
    if(leit==0) pthread_cond_signal(&cond_escr);
    pthread_mutex_unlock(&mutex);
}

// entrada escrita
void InicEscr(int id, int tipo) {
    pthread_mutex_lock(&mutex);
    printf("Thread[%d] de TIPO %d quer escrever\n", id, tipo);
    while((leit>0) || (escr>0)) {
        printf("Thread[%d] de TIPO %d bloqueou\n", id, tipo);
        pthread_cond_wait(&cond_escr, &mutex);
        printf("Thread[%d] de TIPO %d desbloqueou\n", id, tipo);
    }
    escr++;
    pthread_mutex_unlock(&mutex);
}

// saida escrita
void FimEscr(int id, int tipo) {
    pthread_mutex_lock(&mutex);
    printf("Thread[%d] de TIPO %d terminou de escrever\n", id, tipo);
    escr--;
    pthread_cond_signal(&cond_escr);
    pthread_cond_broadcast(&cond_leit);
    pthread_mutex_unlock(&mutex);
}

// thread de tipo 1
void *T1(void *arg) {
    int id = *(int*) arg;
    InicEscr(id, 1);
    global++;
    FimEscr(id, 1);
    pthread_exit(NULL);
}

// thread de tipo 2
void *T2(void *arg) {
    int id = *(int*) arg;
    InicLeit(id, 2);
    if (global % 2 == 0) printf("%d par\n", global);
    else printf("%d impar\n", global);
    FimLeit(id, 2);
    pthread_exit(NULL);
}

// thread de tipo 3
void *T3(void *arg) {
    int id = *(int*) arg;

    InicLeit(id, 3);
    printf("%d\n", global);
    FimLeit(id, 3);
    
    // processamento bobo qualquer (mais significativo)
    for (int i = 0; i < 100000; i++) {}

    InicEscr(id, 3);
    global = id;
    FimEscr(id, 3);

    pthread_exit(NULL);
}

// funcao principal
int main(void) {
    // quantidade total de threads
    int tam = QT1 + QT2 + QT3; 

    // identificadores das threads
    pthread_t tid[tam];
    int id[tam];

    // inicializa as variaveis de sincronizacao
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_leit, NULL);
    pthread_cond_init(&cond_escr, NULL);

    // cria as threads
    for(int i=0; i<QT1; i++) {
        id[i] = i+1;
        if(pthread_create(&tid[i], NULL, T1, (void *) &id[i])) exit(-1);
    }
    for(int i=0; i<QT2; i++) {
        id[i+QT1] = i+1+QT1;
        if(pthread_create(&tid[i], NULL, T2, (void *) &id[i+QT1])) exit(-1);
    }
    for(int i=0; i<QT3; i++) {
        id[i+QT1+QT2] = i+1+QT1+QT2;
        if(pthread_create(&tid[i], NULL, T3, (void *) &id[i+QT1+QT2])) exit(-1);
    }

    // pthread join
    for(int i=0; i<tam; i++) {
        pthread_join(tid[i], NULL);
    }

    // fim
    pthread_exit(NULL);
    return 0;
}
