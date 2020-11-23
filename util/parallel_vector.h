#ifndef PARALLEL_VECTOR_H
#define PARALLEL_VECTOR_H

#include <mutex>
#include <thread>
#include <condition_variable>
#include <exception>
#include <string>

using namespace std;

// errors:
const string VECTOR_STOP = "ParallelStack is in stop "
    "state. This means, that ParallelStack can't push "
    "new values. It can only pop (if not already "
    "empty) and then ParallelStack should be destroyed.";

class Empty : public exception {
    public:
        Empty() : exception(){};
};

/*
class ParallelStack is push-pop data structure
based on std::vector which allows multithread
use of push, pop, size,     stop_vector 
and wake_on_empty_n_waiting methods
*/
template <typename T>
class ParallelStack {
    private:
        bool m_state_stop;
        unsigned m_semaphore;
        unsigned m_wait_limit;
        vector<T> m_vector;
        mutex m_mutex;
        condition_variable m_cv_vector;
        condition_variable m_cv_semaphore;

    public:
        ParallelStack();
        ~ParallelStack();
        T pop_blocking();
        T pop_non_blocking();
        unsigned size() noexcept;
        void push(const T &t_item);
        void stop_vector() noexcept;
        unsigned wake_on_empty_n_waiting
            (unsigned t_waiting_threads) noexcept;

    private:
        inline T pop_without_mutex();
};
#include "parallel_vector.cpp"

// this is duplicit code. There should by some way how to specialize
// parallel_vector on pointer types wihtout this copy-paste by
// using some metaprogramming template technique.
template<typename T>
class ParallelStack<T*>{
    private:
        bool m_state_stop;
        unsigned m_semaphore;
        unsigned m_wait_limit;
        vector<T*> m_vector;
        mutex m_mutex;
        condition_variable m_cv_vector;
        condition_variable m_cv_semaphore;

    public:
        ParallelStack();
        ~ParallelStack();
        T* pop_blocking();
        T* pop_non_blocking();
        unsigned size() noexcept;
        void push(const T* t_item);
        void stop_vector() noexcept;
        unsigned wake_on_empty_n_waiting
            (unsigned t_waiting_threads) noexcept;

    private:
        inline T* pop_without_mutex();
};
#include "parallel_vector_pointer.cpp"

/*
this class could be extended with some other funny stuff
like stop_vector(), resume_vector(), push_non_blocking()
push_if_not_stopped(), pop_if_not_stopped() ...
*/

#endif