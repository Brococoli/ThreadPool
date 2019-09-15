#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <list>

#ifndef TASK_H_
#include "task.h"
#endif

#ifndef THREAD_H_
#include "thread.h"
#endif

#include "utils.h"

class Thread;

class ThreadPool
{
    using func = void *(*)(void*);
public:
    /* ThreadPool():max_idle_(5), max_total_(15) { */
    /*     idle_list_.clear(); */
    /*     total_list_.clear(); */
    /*     task_list_.clear(); */
    /*     total_list_lock_ = PTHREAD_RWLOCK_INITIALIZER; */
    /*     idle_list_lock_ = PTHREAD_MUTEX_INITIALIZER; */
    /* }; */
    ThreadPool(int max_idle = 5, int max_total = 15):
        max_idle_(max_idle), max_total_(max_total)
    {
        idle_list_.clear();
        total_list_.clear();
        task_list_.clear();
        total_list_lock_ = PTHREAD_RWLOCK_INITIALIZER;
        idle_list_lock_ = PTHREAD_MUTEX_INITIALIZER;

        task_cond_lock_ = PTHREAD_COND_INITIALIZER;
        task_mutex_lock_ = PTHREAD_MUTEX_INITIALIZER;

    }
    ~ThreadPool() {
       DestroyThreads(); 
    }


    //this is the five basic step.
    void InitThreads();
    void CreateThreadAndWait();

    void AddTask(Task task);
    void AddTask(func ff, void* arg) { AddTask(Task(ff, arg));};
    int DispatcherTask();  //将任务分发给线程
    int JoinThreads();
    int DestroyThreads();
    int Join();

    void PutIdleList(Thread*); 
    void ModifyThreadNumber();
    bool TaskListEmpty() { return task_list_.size() == 0; }

    void Notify() ;
    void Wait() ;
    void Lock();
    void Unlock();
protected:
    size_t max_idle_;
    size_t max_total_;
    std::list<Thread*> idle_list_;
    std::list<Thread*> total_list_;
    std::list<Task> task_list_;
    pthread_t tid_;


    size_t max_idle() const { return max_idle_; }
    void set_max_idle(const size_t max_idle) { max_idle_ = max_idle; } 
    size_t max_total() const { return max_total_; }
    void set_max_total(const size_t max_total) { max_total_ = max_total; } 
    void PutRunningList(Thread*);
    void PutTotalList(Thread*);
    Thread* GetIdleThread();
    size_t GetTotalThreadNumber();
    size_t GetIdleThreadNumber();

    Thread* AddThread();
    int Lock(pthread_mutex_t* lock);
    int Unlock(pthread_mutex_t* lock);
private:
    static const int ADDSTEP = 5;
    pthread_cond_t task_cond_lock_ ;
    pthread_mutex_t task_mutex_lock_;
    pthread_rwlock_t total_list_lock_;
    pthread_mutex_t idle_list_lock_;

};

void* ThreadPoolMain(void*);
#endif // THREAD_POOL_H_

