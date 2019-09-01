#include "thread_pool_dispatcher.h"

void ThreadPoolDispatcher::Notify() {
    pthread_cond_signal(&task_cond_lock_);
}

void ThreadPoolDispatcher::Wait() {
    pthread_cond_wait(&task_cond_lock_, &task_mutex_lock_);
}

void ThreadPoolDispatcher::AddTask(Task task){
    thread_pool_->AddTask(task);
} 

void ThreadPoolDispatcher::CreateThreadPool(){
    thread_pool_->InitThreads();
    thread_pool_->CreateThreadAndWait(this);
}
