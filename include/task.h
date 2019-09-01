#ifndef TASK_H_
#define TASK_H_
#include <iostream>
class Task
{
public:
    using func = void *(*)(void*);
    Task() { function_ = nullptr; arg_ = nullptr; }
    Task(func function, void* arg): function_(function), arg_(arg) {}
    ~Task() {}
    /* void *(*func_)(void*); */
    func function_;
    void *arg_;

    func function()  { return function_; }
    void set_function(func function) { function_ = function; }

    void* arg() const { return arg_; }
    void set_arg(void* arg) { arg_ = arg; }

    bool Empty() { return function_ == NULL; }

};

#endif  // TASK_H_
