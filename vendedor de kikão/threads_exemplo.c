#include <stdio.h>
#include <pthread.h>

int contador = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* incrementar(void* arg) {
    pthread_mutex_lock(&mutex);
    contador++;
    printf("thread finalizada!\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t threads[5];
    
    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, incrementar, NULL);
    }
    
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Valor final do contador: %d\n", contador);
    
    return 0;
}