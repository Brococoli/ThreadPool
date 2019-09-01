#ifndef THREAD_POOL_DISPATCHER_H_
#define THREAD_POOL_DISPATCHER_H_

#include <pthread.h>

#ifndef THREAD_POOL_H_
#include "thread_pool.h"
#endif

#ifndef TASK_H_
#include "task.h"
#endif

class Thread;
class ThreadPool;


class ThreadPoolDispatcher
{
    using func = void *(*)(void*);
public:
    ThreadPoolDispatcher(ThreadPool* thread_pool){
        thread_pool_ = thread_pool;
        task_cond_lock_ = PTHREAD_COND_INITIALIZER;
        task_mutex_lock_ = PTHREAD_MUTEX_INITIALIZER;
    }
    ~ThreadPoolDispatcher() {}

    void CreateThreadPool();
    void Notify() ;
    void Wait() ;
    void AddTask(func ff, void* arg) { AddTask(Task(ff, arg));};
    void AddTask(Task);

    ThreadPool* thread_pool() { return thread_pool_; }

private:
    pthread_cond_t task_cond_lock_ ;
    pthread_mutex_t task_mutex_lock_;
    ThreadPool* thread_pool_;
};

#endif

