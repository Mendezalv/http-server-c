#ifndef THREAD_POOL_H
#define THREAD_POOL_H

// Inicializa o pool criando N threads que ficarão em background.
void thread_pool_init(int num_threads);

// Função que cada thread executará em loop infinito buscando tarefas.
void* thread_worker(void* arg);

#endif