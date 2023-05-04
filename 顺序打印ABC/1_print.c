#include <func.h>

typedef struct shareRes_s {
    int flag;
    pthread_mutex_t mutex;
    pthread_cond_t cond1;
    pthread_cond_t cond2;
} shareRes_t;

void* func1(void* arg){
   shareRes_t* pshareRes = (shareRes_t*)arg;
   printf("A\n");
   pthread_mutex_lock(&pshareRes->mutex);
   pshareRes->flag = 1;
   pthread_cond_signal(&pshareRes->cond1);
   while (pshareRes->flag == 1){
       pthread_cond_wait(&pshareRes->cond2, &pshareRes->mutex);
   }
   printf("C\n");
   pthread_mutex_unlock(&pshareRes->mutex);
   pthread_exit(NULL);
}

void* func2(void* arg){
    shareRes_t* pshareRes = (shareRes_t*)arg;
    pthread_mutex_lock(&pshareRes->mutex);
    while (pshareRes->flag == 0){
        pthread_cond_wait(&pshareRes->cond1, &pshareRes->mutex);
    }
    printf("B\n");
    pshareRes->flag = 0;
    pthread_cond_signal(&pshareRes->cond2);
    pthread_mutex_unlock(&pshareRes->mutex);
    pthread_exit(NULL);
}

int main()
{
    pthread_t t1, t2;
    shareRes_t shareRes;
    shareRes.flag = 0;
    pthread_mutex_init(&shareRes.mutex, NULL);
    pthread_cond_init(&shareRes.cond1, NULL);
    pthread_cond_init(&shareRes.cond2, NULL);

    pthread_create(&t1, NULL, func1, &shareRes);
    pthread_create(&t2, NULL, func2, &shareRes);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&shareRes.mutex);
    pthread_cond_destroy(&shareRes.cond1);
    pthread_cond_destroy(&shareRes.cond2);
    return 0;
}

