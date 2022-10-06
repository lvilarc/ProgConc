/* Aluno: Lucas Vilar Carneiro DRE 119108874 */
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cond1, cond2;

int count = 0;

/* Tarefa 1 */
void *T1 (void *arg) {
    pthread_mutex_lock(&mutex);
    while(count != 2) {
        pthread_cond_wait(&cond2, &mutex);
    }
    printf("Volte sempre!\n");
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

/* Tarefa 2 */
void *T2 (void *arg) {
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond1, &mutex);
    printf("Fique a vontade.\n");
    count++;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond2);
    pthread_exit(NULL);
}

/* Tarefa 3 */
void *T3 (void *arg) {
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond1, &mutex);
    printf("Sente-se por favor.\n");
    count++;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond2);
    pthread_exit(NULL);
}

/* Tarefa 4 */
void *T4 (void *arg) {
    printf("Seja bem-vindo!\n");
    pthread_cond_broadcast(&cond1);
    pthread_exit(NULL);
}

/* Thread principal */
int main(int argc, char*argv[]) {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond1, NULL);
    pthread_cond_init(&cond2, NULL);
    pthread_t tid_sistema[4];
    pthread_create(&tid_sistema[0], NULL, T1, NULL);
    pthread_create(&tid_sistema[1], NULL, T2, NULL);
    pthread_create(&tid_sistema[2], NULL, T3, NULL);
    pthread_create(&tid_sistema[3], NULL, T4, NULL);
    pthread_join(tid_sistema[0], NULL);
    pthread_join(tid_sistema[1], NULL);
    pthread_join(tid_sistema[2], NULL);
    pthread_join(tid_sistema[3], NULL);
    pthread_exit(NULL);
}



