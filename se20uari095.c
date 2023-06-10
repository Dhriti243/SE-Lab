#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 2

int flag[2];
int turn;
int shared_variable = 0;

void lock_init() {
    flag[0] = flag[1] = 0;
    turn = 0;
}

void lock(int self) {
    flag[self] = 1;
    turn = 1 - self;
    while (flag[1 - self] && turn == 1 - self) {}
}

void unlock(int self) {
    flag[self] = 0;
}

void *thread_function(void *arg) {
    int thread_id = *(int *) arg;
    printf("Thread %d started\n", thread_id);

    for (int i = 0; i < 5; i++) {
        lock(thread_id);
        shared_variable++;
        printf("Thread %d modified shared variable: %d\n", thread_id, shared_variable);
        unlock(thread_id);
    }

    printf("Thread %d finished\n", thread_id);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NTHREADS];
    int thread_ids[NTHREADS] = {0, 1};

    lock_init();

    for (int i = 0; i < NTHREADS; i++) {
        pthread_create(&threads[i], NULL, thread_function, (void *) &thread_ids[i]);
    }

    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Final shared variable value: %d\n", shared_variable);

    return 0;
}