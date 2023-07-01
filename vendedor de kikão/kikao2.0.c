#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <conio.h>

#define MAX_ESTOQUE 10
int quantidade_clientes = 10;
float tempo_medio_atendimento = 0;

typedef struct carrocinha {

    int estoque_materias_primas;
    int clientes_atendidos;

} carrocinha;

carrocinha carroca = {
    10,
    0
};

pthread_mutex_t estoque_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t estoque_vazio = PTHREAD_COND_INITIALIZER;

pthread_cond_t clientes_faltantes = PTHREAD_COND_INITIALIZER;
pthread_mutex_t clientes_atendidos = PTHREAD_MUTEX_INITIALIZER;

void* fornecedor() { //funcão para thread fornecedora de suprimentos
    
    while(true) {
        if(!quantidade_clientes) {
            return NULL;
        }
        int tempo_s = rand() % 5 + 1;
        sleep(tempo_s); // Simula o tempo de entrega do fornecedor
        pthread_mutex_lock(&estoque_mutex);
        
        int aux = MAX_ESTOQUE - carroca.estoque_materias_primas;
        carroca.estoque_materias_primas = MAX_ESTOQUE;

        printf("Pão fornecido: %d unidades.\n", aux);
        printf("Salsicha fornecida: %d unidades.\n", aux);
        printf("Molho fornecido: %d unidades.\n", aux);
        printf("estoque pao: %d, estoque salsicha: %d, estoque molho: %d\n", carroca.estoque_materias_primas, carroca.estoque_materias_primas, carroca.estoque_materias_primas);
        
        pthread_cond_broadcast(&estoque_vazio);
        pthread_mutex_unlock(&estoque_mutex);
    }
}

void* clinte() {
    int tempo_us = (rand() % 451 + 50) * 1000;
    sleep(tempo_us); // Simula o tempo de chegada do cliente

    pthread_mutex_lock(&estoque_mutex); //+

    time_t start_time, end_time;

    double elapsed_time;

    // Obter o tempo de início
    start_time = time(NULL);
    
    if (carroca.estoque_materias_primas == 0) {
        printf("carrocinha sem ingredinentes. cliente aguardando...\n");
        pthread_cond_wait(&estoque_vazio, &estoque_mutex);
    }

    // Obter o tempo de término
    end_time = time(NULL);

    // Calcular o tempo decorrido em segundos
    elapsed_time = difftime(end_time, start_time);

    printf("Tempo decorrido: %.2f segundos\n", elapsed_time);


    }

}




/**
 * 
 * 
 *
 * 
 * 
 * 
 */