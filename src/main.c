#include <stdio.h>
#include "server.h"
#include "thread_pool.h"

#define PORT 8080
#define THREAD_COUNT 4

int main(void) {
    printf("--- C HTTP Server Start ---\n");
    printf("Inicializando Thread Pool com %d workers...\n", THREAD_COUNT);
    
    // Inicializa as threads que aguardarão por tarefas.
    thread_pool_init(THREAD_COUNT);
    
    // Inicia a escuta na porta especificada (Esta função contém um loop infinito).
    start_server(PORT);

    return 0;
}