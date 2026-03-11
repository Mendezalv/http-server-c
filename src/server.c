#include "server.h"
#include "queue.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Importa os mecanismos de sincronização definidos no thread_pool.c.
extern pthread_mutex_t mutex;
extern pthread_cond_t condition_var;

void start_server(int port) {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Cria o descritor do socket (IPv4, TCP).
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Erro na criacao do socket");
        exit(EXIT_FAILURE);
    }

    // Configura o socket para reutilizar a porta imediatamente após ser fechado.
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Configura os parâmetros de rede do servidor.
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Escuta em todas as interfaces de rede locais.
    address.sin_port = htons(port);       // Converte a porta para o formato de rede (Big Endian).

    // Vincula o socket à porta e endereço especificados.
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Erro no bind");
        exit(EXIT_FAILURE);
    }

    // Coloca o socket em modo passivo, aguardando conexões. 
    // O backlog (100) define o tamanho máximo da fila de conexões pendentes do S.O.
    if (listen(server_fd, 100) < 0) {
        perror("Erro no listen");
        exit(EXIT_FAILURE);
    }

    printf("[Server] Escutando conexoes na porta %d...\n", port);

    // Loop principal de aceitação de conexões.
    while (1) {
        // Bloqueia a execução até que um novo cliente se conecte.
        client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        
        if (client_socket < 0) {
            perror("Erro ao aceitar conexao");
            continue;
        }

        // Adiciona a nova conexão à fila de tarefas de forma thread-safe.
        pthread_mutex_lock(&mutex);
        enqueue(client_socket);
        
        // Emite um sinal para acordar uma thread do pool que esteja ociosa.
        pthread_cond_signal(&condition_var); 
        pthread_mutex_unlock(&mutex);
    }
}