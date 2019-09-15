#include "thread.h"


int Thread::CreateAndWait(){
    int ret = pthread_create(&tid_, NULL, DealTask, this);
    if(0 != ret){
        errno = ret;
        err_sys("Syscall Error: pthread_create");
        return ret;
    }
    return 0;
}
int Thread::Detach(){
    return pthread_detach(tid_);
}
int Thread::Join(){
    return pthread_join(tid_, NULL);
}

int Thread::Destroy(){
    return pthread_cancel(tid_);
}


int Thread::Lock(){
    return pthread_mutex_lock(&thread_mutex_lock_);
}
int Thread::Wait(){
    return pthread_cond_wait(&thread_conn_lock_, &thread_mutex_lock_);
}
int Thread::Unlock(){
    return pthread_mutex_unlock(&thread_mutex_lock_);
}
int Thread::Notify(){
    return pthread_cond_signal(&thread_conn_lock_);
}
void* DealTask(void* arg){
    Thread* thread = static_cast<Thread*>(arg);
    for(;;){
        thread->Lock();
        if(thread->task().Empty()){
            /* printf("%ld wait\n", pthread_self()); */
            thread->Wait();
            thread->Unlock();
        }
        else
            thread->Unlock();

        thread->set_running(true); 
        /* printf("%ld run\n", pthread_self()); */
        thread->task().function_(thread->task().arg_);
        thread->set_task(Task());
        thread->set_running(false);

        /* 回调  将自己从忙缓存区拿到空闲缓存区 */
        thread->thread_pool()->PutIdleList(thread);
        thread->set_running(false);
        /* printf("%ld over\n", pthread_self()); */
    }
}
