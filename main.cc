#include <iostream>
#include "thread_pool.h"
#include <unistd.h>
void* do_someting(void * arg){
    long id = (long) arg;
    printf("I am %ld\n", id);
    for(int i = 0; i < 10; ++i)
        printf("%ld: %d\n", id, i);
    return NULL;
}
int main()
{
    printf("Begin:\n");
    ThreadPool tp;
    tp.InitThreads();
    tp.CreateThreadAndWait();
    sleep(1);
    for(long i=0;i<20;i++)
        tp.AddTask(do_someting,(void*) i);

    tp.Lock(); //in case the signal was discard;
    tp.Notify();
    tp.Unlock(); 

    tp.Join();
    printf("End \n");

    return 0;
}

