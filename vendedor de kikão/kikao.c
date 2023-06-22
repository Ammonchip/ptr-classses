#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_ESTOQUE 10

int estoque_pao;
int estoque_salsicha;
int estoque_molho;

pthread_mutex_t estoque_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t estoque_vazio = PTHREAD_COND_INITIALIZER;

void* fornecedor(void* arg) {
    char* ingrediente = (char*)arg;

    while (true) {
        sleep(rand() % 5 + 1); // Simula o tempo de entrega do fornecedor
        
        pthread_mutex_lock(&estoque_mutex);
        
        if (strcmp(ingrediente, "pao") == 0) {
            if (estoque_pao == 0) {
                estoque_pao = MAX_ESTOQUE;
                printf("Pão fornecido: %d unidades.\n", MAX_ESTOQUE);
            }
        }
        else if (strcmp(ingrediente, "salsicha") == 0) {
            if (estoque_salsicha == 0) {
                estoque_salsicha = MAX_ESTOQUE;
                printf("Salsicha fornecida: %d unidades.\n", MAX_ESTOQUE);
            }
        }
        else if (strcmp(ingrediente, "molho") == 0) {
            if (estoque_molho == 0) {
                estoque_molho = MAX_ESTOQUE;
                printf("Molho fornecido: %d unidades.\n", MAX_ESTOQUE);
            }
        }
        
        pthread_cond_broadcast(&estoque_vazio);
        pthread_mutex_unlock(&estoque_mutex);
    }
    
    return NULL;
}

void* cliente(void* arg) {
    usleep((rand() % 451 + 50) * 1000); // Simula o tempo de chegada do cliente
    
    pthread_mutex_lock(&estoque_mutex);
    
    while (estoque_pao == 0 || estoque_salsicha == 0 || estoque_molho == 0) {
        printf("Carrocinha sem ingredientes. Cliente aguardando...\n");
        pthread_cond_wait(&estoque_vazio, &estoque_mutex);
    }
    
    estoque_pao--;
    estoque_salsicha--;
    estoque_molho--;
    
    printf("Kikão vendido e entregue ao cliente.\n");
    
    pthread_mutex_unlock(&estoque_mutex);
    
    return NULL;
}

int main() {
    srand(time(NULL));
    
    pthread_t fornecedor_pao, fornecedor_salsicha, fornecedor_molho;
    pthread_t clientes[10];
    
    pthread_create(&fornecedor_pao, NULL, fornecedor, "pao");
    pthread_create(&fornecedor_salsicha, NULL, fornecedor, "salsicha");
    pthread_create(&fornecedor_molho, NULL, fornecedor, "molho");
    
    for (int i = 0; i < 10; i++) {
        pthread_create(&clientes[i], NULL, cliente, NULL);
    }
    
    pthread_join(fornecedor_pao, NULL);
    pthread_join(fornecedor_salsicha, NULL);
    pthread_join(fornecedor_molho, NULL);
    
    for (int i = 0; i < 10; i++) {
        pthread_join(clientes[i], NULL);
    }
    printf("teste\n");
    return 0;
}
