# C HTTP Server with Thread Pool 🚀

Nota: Este projeto foi desenvolvido como parte da disciplina de APC (Algoritmos e Programação de Computadores) da faculdade.

Este é um servidor HTTP de alta performance escrito inteiramente em C. O projeto foi desenvolvido com foco em **escalabilidade horizontal dentro do processo**, utilizando a arquitetura de **Thread Pool** e o padrão Produtor-Consumidor para gerenciar múltiplas conexões concorrentes sem o overhead de criar uma nova thread para cada requisição.

## 🧠 Arquitetura e Conceitos Utilizados

Este servidor não utiliza bibliotecas de rede de alto nível. Toda a comunicação e gerenciamento de memória foram implementados utilizando as APIs nativas do sistema operacional (POSIX).

* **Sockets POSIX:** Para comunicação TCP/IP na camada de transporte.
* **Thread Pool:** Pré-alocação de um número fixo de *workers* (threads) para processamento paralelo, otimizando o uso de CPU.
* **Sincronização Avançada:** Uso de `pthread_mutex` para garantir a exclusão mútua ao acessar a fila de tarefas, prevenindo *Race Conditions*, e `pthread_cond_t` (Condition Variables) para evitar *busy-waiting*, economizando recursos do sistema.
* **Filas (Linked Lists):** Implementação de uma estrutura de dados FIFO thread-safe para o enfileiramento das requisições pendentes.
* **HTTP Parser Básico:** Leitura e interpretação de requisições `GET`, servindo arquivos estáticos a partir do disco.

## 📂 Estrutura do Projeto

```text
http-server-c/
├── include/
│   ├── http.h          # Headers do interpretador HTTP
│   ├── queue.h         # Headers da Fila thread-safe
│   ├── server.h        # Headers da comunicação TCP via Sockets
│   └── thread_pool.h   # Headers do gerenciamento de threads
├── src/
│   ├── http.c          # Lógica de resposta e roteamento HTTP
│   ├── main.c          # Ponto de entrada e inicialização global
│   ├── queue.c         # Implementação da Fila de tarefas (malloc/free)
│   ├── server.c        # Configuração do Socket (bind, listen, accept)
│   └── thread_pool.c   # Ciclo de vida dos workers (Mutex/Condition Var)
├── www/
│   └── index.html      # Arquivos estáticos servidos pelo servidor
├── Makefile            # Automação da compilação
└── README.md