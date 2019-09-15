#include "thread_pool.h"

void ThreadPool::InitThreads(){
    for (size_t i = 0; i < max_idle(); ++i){
        Thread*  thread = new Thread(this);
        idle_list_.push_back(thread);
        total_list_.push_back(thread);
        thread->CreateAndWait();
    }
}

size_t ThreadPool::GetIdleThreadNumber(){
    int res = -1;
    Lock(&idle_list_lock_);
    res = idle_list_.size();
    Unlock(&idle_list_lock_);
    return res;

}
void ThreadPool::PutIdleList(Thread* thread){
    Lock(&idle_list_lock_);
    if(GetTotalThreadNumber() < max_total()) {
        if(idle_list_.size() < max_idle())
            idle_list_.push_back(thread);
    }
    Unlock(&idle_list_lock_);
}
void ThreadPool::PutTotalList(Thread* thread){
    pthread_rwlock_wrlock(&total_list_lock_);
    if(GetTotalThreadNumber() < max_total_) //
        total_list_.push_back(thread);
    pthread_rwlock_unlock(&total_list_lock_);
}

int ThreadPool::Lock(pthread_mutex_t* lock){
    return pthread_mutex_lock(lock);
}
int ThreadPool::Unlock(pthread_mutex_t* lock){
    return pthread_mutex_unlock(lock);
}
Thread* ThreadPool::GetIdleThread()  {
    Thread* res = nullptr;
    Lock(&idle_list_lock_);
    if(idle_list_.size() != 0) {
        res = idle_list_.front();
        idle_list_.pop_front();
    }
    Unlock(&idle_list_lock_);
    return res;
}

size_t ThreadPool::GetTotalThreadNumber(){
    size_t ans = -1;
    pthread_rwlock_rdlock(&total_list_lock_);
    ans = total_list_.size();
    pthread_rwlock_unlock(&total_list_lock_);
    return ans;
}

void ThreadPool::AddTask(Task task){
    task_list_.push_back(task);
}
Thread* ThreadPool::AddThread(){
    int add_number = ADDSTEP;
    Thread* thread = nullptr, *temp=nullptr;
    for(int i = 0; GetTotalThreadNumber() < max_total_ && i < add_number; ++i){
        temp = new Thread(this);
        PutIdleList(temp);
        if(i)  thread = temp;
    }
    return thread;
}
int ThreadPool::DispatcherTask(){
    Thread* thread = nullptr;
    if((thread = GetIdleThread()) == nullptr){ //get a idle thread.
        return 0;
    }

    Task task;
    if(!task_list_.empty()){  // select a task. (FIFO)
        task = task_list_.front();
        task_list_.pop_front();
    }
    else
        return -1;

    /* thread->Detach(); */
    thread->set_task(task); //tell the thread its task.
    thread->Lock();
    thread->Notify();  // tell he to do it!.
    thread->Unlock();
    return 1;
}

void ThreadPool::ModifyThreadNumber(){  //不能使用Add'Thead，要保证查询大小和删除或添加线程的原子性
    bool add_thread = task_list_.size() != 0;
    size_t idle_size, total_size;
    Lock(&idle_list_lock_);
    pthread_rwlock_rdlock(&total_list_lock_);
    idle_size = idle_list_.size();
    total_size = total_list_.size();
    if(add_thread){
        if(idle_size < max_idle() && total_list_.size() < max_total()){
            idle_list_.push_back(new Thread(this));
        }
    }
    else{
        if(idle_size > max_idle() || total_list_.size() > max_total()){
            delete idle_list_.front();
            idle_list_.pop_front();
        }
    }
    Unlock(&idle_list_lock_);
    pthread_rwlock_unlock(&total_list_lock_);
}

void ThreadPool::CreateThreadAndWait(){
    pthread_create(&tid_, NULL, ThreadPoolMain, (void*)this);
}
void* ThreadPoolMain(void* arg){
    /* printf("into ThreadPoolMain\n"); */
    ThreadPool* tp = static_cast<ThreadPool*>(arg);
    for(;;){
        tp->Lock();
        if(tp->TaskListEmpty()){
            /* printf("threadPool wait\n"); */
            tp->Wait();  //wait for task comming;
            tp->Unlock();
        }
        else
            tp->Unlock();
        /* printf("threadPool DispatcherTask\n"); */
        tp->DispatcherTask();
        /* printf("ModifyThreadNumber\n"); */
        tp->ModifyThreadNumber();
        /* printf("threadPool over\n"); */

    }
}
int ThreadPool::JoinThreads(){
    int ok = 1;
    pthread_rwlock_rdlock(&total_list_lock_);
    for(Thread* thread : total_list_){
        if(!thread->running()) continue;
        if(thread->Join() != 0){
            err_ret("JoinThreads error");
            ok = -1;
        }
    }
    pthread_rwlock_unlock(&total_list_lock_);
    return ok;
}
int ThreadPool::DestroyThreads(){
    int ok = 1;
    Thread* thread;
    pthread_rwlock_wrlock(&total_list_lock_);
    while(!total_list_.empty()){
        thread = total_list_.front();
        if(thread->Destroy() != 0){
            err_ret("Destroy error");
            ok = -1;
        }
        else{
            delete thread;
            total_list_.pop_front();
        }
    }
    pthread_rwlock_unlock(&total_list_lock_);
    return ok;

}
int ThreadPool::Join(){
    return pthread_join(tid_, NULL);
}
void ThreadPool::Notify() {
    pthread_cond_signal(&task_cond_lock_);
}
void ThreadPool::Lock(){
    pthread_mutex_lock(&task_mutex_lock_);
}
void ThreadPool::Wait() {
    pthread_cond_wait(&task_cond_lock_, &task_mutex_lock_);
}

void ThreadPool::Unlock() {
    pthread_mutex_unlock(&task_mutex_lock_);
}

