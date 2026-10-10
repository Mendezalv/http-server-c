# C HTTP Server

Servidor HTTP concorrente implementado em C utilizando as APIs nativas do sistema operacional (POSIX Sockets e Pthreads). O projeto gerencia múltiplas conexões concorrentes através de um modelo de **Thread Pool** associado ao padrão **Produtor-Consumidor**, evitando o overhead de criação e destruição de threads por requisição.

> **Contexto:** Desenvolvido no âmbito da disciplina de Algoritmos e Programação de Computadores (APC).

---

## Arquitetura e Conceitos

O servidor opera sem dependência de bibliotecas de rede de alto nível:

- **Sockets POSIX:** Comunicação TCP/IP na camada de transporte via chamadas de sistema `socket`, `bind`, `listen` e `accept`.
- **Thread Pool:** Pré-alocação de um conjunto fixo de threads operárias (*workers*) inicializadas no startup da aplicação para consumo de tarefas da fila.
- **Sincronização com Pthreads:**
  - `pthread_mutex_t`: Garante exclusão mútua no acesso à fila de conexões pendentes, prevenindo condições de corrida (*race conditions*).
  - `pthread_cond_t`: Uso de variáveis de condição para notificação orientada a eventos, eliminando espera ocupada (*busy-waiting*) e otimizando o uso de CPU.
- **Fila Dinâmica Thread-Safe:** Estrutura de dados encadeada (FIFO) com alocação dinâmica (`malloc`/`free`) para enfileiramento das conexões aceitas pelo listener principal.
- **Parser e Despache HTTP:** Extração e interpretação de requisições `GET`, com envio dos headers HTTP apropriados e entrega de arquivos estáticos a partir do diretório `www/`.

---

## Estrutura do Projeto

```text
http-server-c/
├── include/
│   ├── http.h          # Definições do parser e roteamento HTTP
│   ├── queue.h         # Estrutura e assinaturas da fila thread-safe
│   ├── server.h        # Configuração do socket TCP e loop de escuta
│   └── thread_pool.h   # Inicialização e ciclo de vida dos workers
├── src/
│   ├── http.c          # Roteamento de arquivos e escrita de respostas HTTP
│   ├── main.c          # Ponto de entrada e inicialização dos subsistemas
│   ├── queue.c         # Gerenciamento da fila encadeada de conexões
│   ├── server.c        # Configuração do socket servidor (porta 8080)
│   └── thread_pool.c   # Execução dos workers com mutex e condition variables
├── www/
│   └── index.html      # Arquivos estáticos servidos pelo servidor
├── Makefile            # Diretivas de compilação
└── README.md
```

---

## Compilação e Execução

### Pré-requisitos
- Compilador C (`gcc` ou `clang`) com suporte a C99/C11.
- Utilitário `make`.
- Ambiente POSIX (Linux, macOS ou WSL no Windows).

### Compilar
```bash
make
```

### Executar
```bash
./http_server
# ou diretamente via Makefile:
make run
```
O servidor será inicializado na porta `8080`.

### Testar Conexão
Em outro terminal, execute uma requisição HTTP via `curl`:
```bash
curl -i http://localhost:8080/
```

### Limpar Artefatos de Build
```bash
make clean
```
