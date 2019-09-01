#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>

#ifndef TASK_H_
#include "task.h"
#endif

#ifndef UTILS_H_
#include "utils.h"
#endif

#ifndef THREAD_POOL_H_
#include "thread_pool.h"
#endif

class ThreadPool;

class Thread
{
public:
    /* Thread(const ThreadPool* tp): thread_pool_(tp) { */
    /*     running_ = detached_ = false; */
    /*     thread_lock_ = PTHREAD_MUTEX_INITIALIZER; */
    /*     task_ = Task(); */
    /* } */ 
    Thread(ThreadPool* tp){
        thread_pool_ = tp;
        running_ = detached_ = false;
        thread_mutex_lock_ = PTHREAD_MUTEX_INITIALIZER;
        thread_conn_lock_ = PTHREAD_COND_INITIALIZER;
        task_ = Task();
    } 
    ~Thread() {}

    bool running() const { return running_; } // if it is alive
    void set_running(bool running) { running_ = running; }
    bool detached() const { return detached_; }
    void set_detached(bool detached) { detached_ = detached; }


    int CreateAndWait(); // create a new thread
    int Detach();
    int Join();
    int Destroy();
    pthread_t tid() const { return tid_; }// get the thread tid
    Task task() const { return task_; }
    void set_task(Task task) { task_ = task; }
    ThreadPool* thread_pool() { return thread_pool_; }

    int Wait();
    int Lock();
    int Unlock();
    int Notify();

private:
    pthread_t tid_;  // the thread's tid
    bool running_; // if it is running
    bool detached_; //if it has detached
    Task task_;
    ThreadPool* thread_pool_;
    pthread_mutex_t thread_mutex_lock_;
    pthread_cond_t thread_conn_lock_;

};

void* DealTask(void *);
#endif
