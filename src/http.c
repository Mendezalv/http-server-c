#include "http.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

void handle_connection(int client_socket) {
    char buffer[2048];
    
    // Lê os dados crus enviados pelo navegador/cliente.
    int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) return;
    buffer[bytes_read] = '\0'; // Assegura que o buffer é uma string C válida.

    // Realiza o parse da primeira linha da requisição HTTP.
    char method[10], path[255], protocol[20];
    sscanf(buffer, "%s %s %s", method, path, protocol);
    printf("[HTTP] Request recebido: %s %s\n", method, path);

    // Roteamento básico: direciona requisições à raiz para o index.html.
    if (strcmp(path, "/") == 0) {
        strcpy(path, "/index.html");
    }

    // Constrói o caminho relativo do arquivo no sistema de arquivos local.
    char full_path[300];
    snprintf(full_path, sizeof(full_path), "www%s", path);

    // Tenta abrir o arquivo solicitado em modo somente leitura (O_RDONLY).
    int file = open(full_path, O_RDONLY);
    
    if (file == -1) {
        // Se o arquivo não existir, retorna um erro HTTP 404.
        char *not_found_response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n<h1>404 - Pagina nao encontrada</h1>";
        write(client_socket, not_found_response, strlen(not_found_response));
    } else {
        // Se o arquivo existir, envia o cabeçalho HTTP 200 OK.
        // A sequência \r\n\r\n separa o cabeçalho do corpo da resposta.
        char *success_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        write(client_socket, success_header, strlen(success_header));
        
        // Lê o arquivo em blocos de 1024 bytes e envia pelo socket sequencialmente.
        char file_buffer[1024];
        int bytes;
        while ((bytes = read(file, file_buffer, sizeof(file_buffer))) > 0) {
            write(client_socket, file_buffer, bytes);
        }
        
        // Fecha o descritor de arquivo local após a transferência.
        close(file);
    }
}