#include "parallel_vector.h"

template <typename T>
ParallelVector<T>::ParallelVector() : m_state_stop(false){};

template <typename T>
inline T ParallelVector<T>::pop_without_mutex()
{
    if (m_vector.empty())
    {
        throw Empty();
    }

    T ret_val = m_vector.back();
    m_vector.pop_back();
    return ret_val;
}

template <typename T>
T ParallelVector<T>::pop_blocking()
{
    unique_lock<mutex> lock(m_mutex);
    m_cond_var.wait(lock, [&]() { return !m_vector.empty() || m_state_stop; });
    return pop_without_mutex();
}

template <typename T>
T ParallelVector<T>::pop_non_blocking()
{
    unique_lock<mutex> lock(m_mutex);
    return pop_without_mutex();
}

template <typename T>
void ParallelVector<T>::stop_vector()
{
    {
        unique_lock<mutex> lock(m_mutex);
        m_state_stop = true;
    }
    m_cond_var.notify_all();
}

template <typename T>
void ParallelVector<T>::push(const T &t_item)
{
    {
        unique_lock<mutex> lock(m_mutex);
        if (m_state_stop) {
            throw logic_error(VECTOR_STOP);
        } else {
            m_vector.push_back(t_item);
        }
    }
    m_cond_var.notify_one();
}

template <typename T>
ParallelVector<T>::~ParallelVector() 
{
    stop_vector();
}