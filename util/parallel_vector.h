#ifndef PARALLEL_VECTOR_H
#define PARALLEL_VECTOR_H

#include <mutex>
#include <thread>
#include <condition_variable>
#include <exception>
#include <string>

using namespace std;

// errors:
const string VECTOR_STOP = "ParallelVector is in stop "
    "state. This means, that ParallelVector can't push "
    "new values. It can only pop (if not already "
    "empty) and then ParallelVector should be destroyed.";

class Empty : public exception {
    public:
        Empty() : exception(){};
};


template <typename T>
class ParallelVector {
    private:
        bool m_state_stop;
        unsigned m_semaphore;
        unsigned m_wait_limit;
        vector<T> m_vector;
        mutex m_mutex;
        condition_variable m_cv_vector;
        condition_variable m_cv_semaphore;

    public:
        ParallelVector();
        ~ParallelVector();
        T pop_blocking();
        T pop_non_blocking();
        void push(const T &t_item);
        void stop_vector();
        unsigned wake_on_empty_n_waiting
            (unsigned t_waiting_threads);

    private:
        inline T pop_without_mutex();
};
#include "parallel_vector.cpp"

/*
this class could be extended with some other funny stuff
like stop_vector(), resume_vector(), push_non_blocking()
push_if_not_stopped(), pop_if_not_stopped() ...
*/

#endif