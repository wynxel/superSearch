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
class ParallelVector
{
    private:
        bool m_state_stop;
        vector<T> m_vector;
        mutex m_mutex;
        condition_variable m_cond_var;

    public:
        ParallelVector();
        ~ParallelVector();
        T pop_blocking();
        T pop_non_blocking();
        void stop_vector();
        void push(const T &t_item);

    private:
        inline T pop_without_mutex();
};
#include "parallel_vector.cpp"
#endif