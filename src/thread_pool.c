#include "thread_pool.h"
#include "queue.h"
#include "http.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Mutex: Garante exclusão mútua. Apenas uma thread manipula a fila por vez.
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Condition Variable: Permite que as threads durmam até que haja uma nova tarefa.
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;

void* thread_worker(void* arg) {
    while (1) {
        int* p_client;

        // Bloqueia o mutex antes de acessar a fila compartilhada.
        pthread_mutex_lock(&mutex);

        // Se a fila estiver vazia, a thread entra em estado de espera (sleep).
        // Ela será acordada quando o pthread_cond_signal for chamado no server.c.
        if ((p_client = dequeue()) == NULL) {
            pthread_cond_wait(&condition_var, &mutex);
            // Ao acordar, tenta retirar a tarefa da fila novamente.
            p_client = dequeue();
        }

        // Libera o mutex para que outras threads possam acessar a fila.
        pthread_mutex_unlock(&mutex);

        // Se conseguiu pegar uma conexão válida, inicia o processamento.
        if (p_client != NULL) {
            printf("[Thread Pool] Worker %lu assumiu a conexao %d\n", pthread_self(), *p_client);
            
            // Repassa o socket para o interpretador HTTP.
            handle_connection(*p_client); 
            
            // Encerra a conexão com o cliente após a resposta ser enviada.
            close(*p_client); 
            free(p_client);
        }
    }
    return NULL;
}

void thread_pool_init(int num_threads) {
    pthread_t threads[num_threads];
    // Cria as threads e as direciona para a função thread_worker.
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, thread_worker, NULL);
    }
}