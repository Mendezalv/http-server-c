#include "queue.h"
#include <stdlib.h>

// Inicializa a fila global como vazia.
static queue_t q = {NULL, NULL};

void enqueue(int client_socket) {
    // Aloca memória para o novo nó que entrará na fila.
    node_t* new_node = malloc(sizeof(node_t));
    new_node->client_socket = client_socket;
    new_node->next = NULL;

    // Se a fila estiver vazia, o novo nó será o primeiro (head).
    if (q.tail == NULL) {
        q.head = new_node;
    } else {
        // Caso contrário, encadeia o novo nó após o último atual.
        q.tail->next = new_node;
    }
    // Atualiza o ponteiro de fim da fila (tail) para o novo nó.
    q.tail = new_node;
}

int* dequeue() {
    // Retorna nulo se não houver clientes na fila.
    if (q.head == NULL) return NULL;

    // Aloca memória apenas para retornar o valor numérico do socket.
    int* result = malloc(sizeof(int));
    *result = q.head->client_socket;

    // Salva a referência do nó atual para podermos liberar a memória.
    node_t* temp = q.head;
    
    // Avança a cabeça da fila para o próximo nó.
    q.head = q.head->next;
    
    // Se a fila esvaziar completamente, precisamos zerar o tail também.
    if (q.head == NULL) q.tail = NULL;

    // Libera o nó antigo da memória.
    free(temp);
    
    return result;
}