#include <func.h>

pthread_mutex_t mutex;
bool flag = false;

void *threadFuncA(void* arg){
    pthread_mutex_lock(&mutex);
    if (flag == false){
        printf("thread A is working\n");
        flag = true;
        sleep(3);
        printf("thread A has done\n");
        pthread_mutex_unlock(&mutex);
    }
    else{
        pthread_mutex_unlock(&mutex);
    }
}

void *threadFuncB(void* arg){
    pthread_mutex_lock(&mutex);
    while (flag == false){
        pthread_mutex_unlock(&mutex);
        sleep(1);
        pthread_mutex_lock(&mutex);
    }
    printf("thread B is coming now\n");
    pthread_mutex_unlock(&mutex);
}

int main()
{
    pthread_t t1, t2;
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&t1, NULL, threadFuncA, NULL);
    pthread_create(&t2, NULL, threadFuncB, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_mutex_destroy(&mutex);
    return 0;
}

