#include <func.h>
#include "Queue.h"

#define MAX_SIZE 10
#define DEFAULT_SIZE 8

pthread_mutex_t mutex;
pthread_cond_t cond;

void produce(Queue* q){
    int value = rand() % 100 + 1;
    pthread_mutex_lock(&mutex);
    while (q->size >= q->capacity){
        printf("queue is full, waiting\n");
        pthread_cond_wait(&cond, &mutex);
    }
    push_queue(q, value);
    printf("Produced: %d\n", value);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}

void consume(Queue* q){
    pthread_mutex_lock(&mutex);
    while (q->size <= 0){
        printf("queue is empty, waiting\n");
        pthread_cond_wait(&cond, &mutex);
    }
    int popVal = pop_queue(q);
    printf("Consumed: %d\n", popVal);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}

void* producer(void* arg){
    Queue* q = (Queue*)arg;
    while (1){
        produce(q);
        sleep(3);
    }
}

void* consumer(void* arg){
    Queue* q = (Queue*)arg;
    sleep(5);
    while (1){
        consume(q);
        sleep(1);
    }
}

int main()
{
    srand(time(NULL));
    Queue* q = create_queue(MAX_SIZE);
    for (int i = 0; i < DEFAULT_SIZE; ++i){
       int value = rand() % 100 + 1;
       push_queue(q, value);
       visit(q);
    }
    pthread_t tp1, tp2, tp3, tc1, tc2;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_create(&tp1, NULL, producer, q);
    pthread_create(&tp2, NULL, producer, q);
    pthread_create(&tp3, NULL, producer, q);
    pthread_create(&tc1, NULL, consumer, q);
    pthread_create(&tc2, NULL, consumer, q);
    pthread_join(tp1, NULL);
    pthread_join(tp2, NULL);
    pthread_join(tp3, NULL);
    pthread_join(tc1, NULL);
    pthread_join(tc2, NULL);
    return 0;
}

