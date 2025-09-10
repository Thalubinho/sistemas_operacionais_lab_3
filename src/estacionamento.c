#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <limits.h>

#define NUM_VAGAS 5
#define NUM_CARROS 10

int vagas_disponiveis = NUM_VAGAS;
pthread_mutex_t mutex_vagas;

volatile int futex_vagas = 0;

// Wrapper para a chamada de sistema futex
int futex(volatile int *uaddr, int op, int val, const struct timespec *timeout, int *uaddr2, int val3) {
    return syscall(SYS_futex, uaddr, op, val, timeout, uaddr2, val3);
}

// Função para simular a entrada de um carro
void entrar_estacionamento(int id) {
    printf("Carro %d chegou e esta procurando uma vaga.\n", id);

    while (1) {
        pthread_mutex_lock(&mutex_vagas);

        if (vagas_disponiveis > 0) {
            vagas_disponiveis--;
            printf("Carro %d encontrou uma vaga. Vagas restantes: %d\n", id, vagas_disponiveis);
            pthread_mutex_unlock(&mutex_vagas);
            break; 
        }

        pthread_mutex_unlock(&mutex_vagas);

        printf("Carro %d: Estacionamento cheio, esperando por uma vaga...\n", id);
        futex(&futex_vagas, FUTEX_WAIT, 0, NULL, NULL, 0);
    }
}

// Função para simular a saída de um carro
void sair_estacionamento(int id) {
    sleep(1 + rand() % 3); // Simula o tempo que o carro fica estacionado

    pthread_mutex_lock(&mutex_vagas);
    vagas_disponiveis++;
    printf("Carro %d saiu. Vagas restantes: %d\n", id, vagas_disponiveis);
    pthread_mutex_unlock(&mutex_vagas);

    futex(&futex_vagas, FUTEX_WAKE, 1, NULL, NULL, 0);
}

void *carro_thread(void *arg) {
    int id = *((int *)arg);

    entrar_estacionamento(id);
    sair_estacionamento(id);

    free(arg);
    return NULL;
}

int main() {
    pthread_t threads[NUM_CARROS];
    int i;

    pthread_mutex_init(&mutex_vagas, NULL);

    srand(time(NULL));

    printf("Estacionamento com %d vagas.\n", NUM_VAGAS);

    for (i = 0; i < NUM_CARROS; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&threads[i], NULL, carro_thread, id);
    }

    for (i = 0; i < NUM_CARROS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex_vagas);

    printf("Todos os carros completaram suas tarefas.\n");

    return 0;
}