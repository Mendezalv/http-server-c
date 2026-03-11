#ifndef HTTP_H
#define HTTP_H

// Lê a requisição HTTP do socket, identifica o arquivo e envia a resposta.
void handle_connection(int client_socket);

#endif