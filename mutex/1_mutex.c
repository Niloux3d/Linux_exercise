#include <func.h>

#define NUM 10000000

int global = 0;
pthread_mutex_t mutex;

void* threadFunc(void* arg){
    struct timeval start, end;
    gettimeofday(&start, NULL);
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < NUM; ++i){
        ++global;
    }
    pthread_mutex_unlock(&mutex);
    gettimeofday(&end, NULL);
    double timecost = end.tv_sec - start.tv_sec +
                     (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("child thread cost %f秒\n", timecost);
}

int main()
{   
    struct timeval start, end;

    pthread_t tid;
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&tid, NULL, threadFunc, NULL);
    gettimeofday(&start, NULL);
    for (int i = 0; i < NUM; ++i){
        pthread_mutex_lock(&mutex);
        ++global;
        pthread_mutex_unlock(&mutex);
    }
    gettimeofday(&end, NULL);
    double timecost = end.tv_sec - start.tv_sec +
                     (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("main thread cost%f秒\n", timecost);
    pthread_join(tid, NULL);

    pthread_mutex_destroy(&mutex);
    printf("global = %d\n", global);
    return 0;
}

