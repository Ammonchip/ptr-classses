#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_INGREDIENTE 5
#define MAX_CLIENTES 20
int estoque_pao = MAX_INGREDIENTE;
int estoque_salsicha = MAX_INGREDIENTE;
int estoque_molho = MAX_INGREDIENTE;

pthread_mutex_t mutex_pao;
pthread_mutex_t mutex_salsicha;
pthread_mutex_t mutex_molho;
pthread_mutex_t mutex_atendimento;

pthread_cond_t cond_pao;
pthread_cond_t cond_salsicha;
pthread_cond_t cond_molho;
pthread_cond_t cond_atendimento;

int cliente_atual = 1;
int cliente_atendido = 0;
int qtd_clientes = MAX_CLIENTES;
int total_clientes = MAX_CLIENTES;
long int tempo_espera_total = 0;

void* fornecedor_pao(void* arg) {
    while (qtd_clientes>0) {
        if (estoque_pao < MAX_INGREDIENTE) {
            pthread_mutex_lock(&mutex_pao);
            estoque_pao = MAX_INGREDIENTE;
            printf("Fornecedor de pao/: abastece estoque de pao. Estoque de pao: %d\n", estoque_pao);
            if (estoque_pao >= 1)
                pthread_cond_signal(&cond_pao);
        }
        pthread_mutex_unlock(&mutex_pao);
        usleep(rand() % 1000000 + 3000000); // Tempo de entrega aleatório entre 1s e 3s
    }
    return NULL;
}

void* fornecedor_salsicha(void* arg) {
    while (qtd_clientes>0) {
        if (estoque_salsicha < MAX_INGREDIENTE) {
            pthread_mutex_lock(&mutex_salsicha);
            estoque_salsicha = MAX_INGREDIENTE;
            printf("Fornecedor de salsicha: abastece estoque de salsicha. Estoque de salsicha: %d\n", estoque_salsicha);
            if (estoque_salsicha >= 1)
                pthread_cond_signal(&cond_salsicha);
        }
        pthread_mutex_unlock(&mutex_salsicha);
        usleep(rand() % 1000000 + 3000000); // Tempo de entrega aleatório entre 500ms e 1s
    }
    return NULL;
}

void* fornecedor_molho(void* arg) {
    while (qtd_clientes>0) {
        if (estoque_molho < MAX_INGREDIENTE) {
            pthread_mutex_lock(&mutex_molho);
            estoque_molho = MAX_INGREDIENTE;
            printf("Fornecedor de molho: abastece estoque de molho. Estoque de molho: %d\n", estoque_molho);
            if (estoque_molho >= 1)
                pthread_cond_signal(&cond_molho);
        }
        pthread_mutex_unlock(&mutex_molho);
        usleep(rand() % 1000000 + 3000000); // Tempo de entrega aleatório entre 500ms e 1s
    }
    return NULL;
}

void* cliente(void* arg) {
    int id = *(int*)arg;
    free(arg);

    time_t start_time, end_time;

    double elapsed_time;

    // Obter o tempo de início
    start_time = time(NULL);
    printf("cliente %d chegou! tempo de chegada: %d\n", id, start_time);
    while(cliente_atual != id) {
        pthread_cond_wait(&cond_atendimento, &mutex_atendimento);
    }
    
    pthread_mutex_lock(&mutex_pao);
    while (estoque_pao < 1) {
        pthread_cond_wait(&cond_pao, &mutex_pao);
    }
    estoque_pao--;
    pthread_mutex_unlock(&mutex_pao);

    pthread_mutex_lock(&mutex_salsicha);
    while (estoque_salsicha < 1) {
        pthread_cond_wait(&cond_salsicha, &mutex_salsicha);
    }
    estoque_salsicha--;
    pthread_mutex_unlock(&mutex_salsicha);

    pthread_mutex_lock(&mutex_molho);
    while (estoque_molho < 1) {
        pthread_cond_wait(&cond_molho, &mutex_molho);
    }
    estoque_molho--;
    pthread_mutex_unlock(&mutex_molho);

    printf("Cliente %d: Preparando Kikao...\n", id);
    usleep(100000); // Tempo de preparação do Kikão (100ms)
    // Obter o tempo de término
    end_time = time(NULL);

    // Calcular o tempo decorrido em segundos
    elapsed_time = difftime(end_time, start_time);
    printf("Cliente %d: Kikao preparado e consumido.\n", id);

    pthread_mutex_lock(&mutex_pao);
    pthread_cond_signal(&cond_pao);
    pthread_mutex_unlock(&mutex_pao);

    pthread_mutex_lock(&mutex_salsicha);
    pthread_cond_signal(&cond_salsicha);
    pthread_mutex_unlock(&mutex_salsicha);

    pthread_mutex_lock(&mutex_molho);
    pthread_cond_signal(&cond_molho);
    pthread_mutex_unlock(&mutex_molho);

    
    tempo_espera_total += elapsed_time;
    cliente_atual++;
    qtd_clientes--;
    pthread_exit(NULL);



}

int main() {
    pthread_t fornecedor_pao_thread, fornecedor_salsicha_thread, fornecedor_molho_thread;
    pthread_t clientes_thread[qtd_clientes];

    pthread_mutex_init(&mutex_pao, NULL);
    pthread_mutex_init(&mutex_salsicha, NULL);
    pthread_mutex_init(&mutex_molho, NULL);

    pthread_cond_init(&cond_pao, NULL);
    pthread_cond_init(&cond_salsicha, NULL);
    pthread_cond_init(&cond_molho, NULL);

    pthread_create(&fornecedor_pao_thread, NULL, fornecedor_pao, NULL);
    pthread_create(&fornecedor_salsicha_thread, NULL, fornecedor_salsicha, NULL);
    pthread_create(&fornecedor_molho_thread, NULL, fornecedor_molho, NULL);

    for (int i = 0; i < total_clientes; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        int tempo_chegada_us = (rand() % 451 + 50) * 1000;
        usleep(tempo_chegada_us); // Simula o tempo de chegada do cliente
        pthread_create(&clientes_thread[i], NULL, cliente, id);
    }
    pthread_join(fornecedor_pao_thread, NULL);
    pthread_join(fornecedor_salsicha_thread, NULL);
    pthread_join(fornecedor_molho_thread, NULL);
    for (int i = 0; i < 10; i++) {
        pthread_join(clientes_thread[i], NULL);
    }

    double tempo_medio_espera = (double)tempo_espera_total / qtd_clientes;
    printf("Tempo medio de espera: %.2f ms\n", tempo_medio_espera);

    pthread_mutex_destroy(&mutex_pao);
    pthread_mutex_destroy(&mutex_salsicha);
    pthread_mutex_destroy(&mutex_molho);

    pthread_cond_destroy(&cond_pao);
    pthread_cond_destroy(&cond_salsicha);
    pthread_cond_destroy(&cond_molho);

    return 0;
}
