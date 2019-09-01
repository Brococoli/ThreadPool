#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
void* do_it(void * arg){
    printf("create thread\n");
    pthread_cond_t* cond = (pthread_cond_t*)(arg);
    /* for(int i=0;i<2;i++){ */
        /* pthread_mutex_lock(&mutex); */
        pthread_cond_signal(cond);
        /* pthread_mutex_unlock(&mutex); */
        printf("send signal\n");
        /* sleep(3); */
    /* } */
    return NULL;
}
int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, do_it, &cond);
    pthread_cond_signal(&cond);
    for(int i = 0; i < 2 ; ++i){
        printf("lock\n");
        pthread_mutex_lock(&mutex);
        sleep(5);
        pthread_cond_wait(&cond, &mutex);
        pthread_mutex_unlock(&mutex);
        printf("unlock\n");
    }
    printf("end\n");

    pthread_join(tid, NULL);
    return 0;
}

