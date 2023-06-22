#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_ESTOQUE 10

typedef struct carrocinha {

    int estoque_pao;
    int estoque_salsicha;
    int estoque_molho;
} carrocinha;

carrocinha carroca = {
    10,
    10,
    10
};

pthread_mutex_t estoque_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t estoque_vazio = PTHREAD_COND_INITIALIZER;

void* fornecedor_pao(void* arg) { //funcão para thread fornecedora de pao
    sleep(rand() % 5 + 1); // Simula o tempo de entrega do fornecedor
    pthread_mutex_lock(&estoque_mutex);
    carroca.estoque_pao = MAX_ESTOQUE;
    printf("pao reabastecido!\n");
    printf("estoque pao: %d, estoque salsicha: %d, estoque molho: %d\n", carroca.estoque_pao, carroca.estoque_salsicha, carroca.estoque_molho);
    
}

//parei com o seguinte problema, tenho que dizer para thread forncedora quando parar
//pensei na seguinte solução: criar pthread_cond_t que diz quando todos os clientes 
//foram atendidos