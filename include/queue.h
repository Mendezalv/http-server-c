#ifndef QUEUE_H
#define QUEUE_H

// Estrutura de um nó da lista encadeada.
// Armazena o descritor do socket do cliente e aponta para o próximo da fila.
typedef struct node {
    int client_socket;
    struct node* next;
} node_t;

// Estrutura que gerencia o início (head) e o fim (tail) da fila.
typedef struct {
    node_t *head;
    node_t *tail;
} queue_t;

// Adiciona um novo socket ao final da fila.
void enqueue(int client_socket);

// Remove e retorna o socket que está no início da fila.
int* dequeue();

#endif