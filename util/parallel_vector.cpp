#include "parallel_vector.h"

template <typename T>
ParallelVector<T>::ParallelVector() : 
    m_state_stop(false),
    m_semaphore(0),
    m_wait_limit(0) {};

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
    m_semaphore++;
    if (m_wait_limit == m_semaphore) {
        // info: m_wait_limit should be tested on non-zero
        // (performance improvement), but as m_semaphore is ++
        // one line above, m_cv... will not be notified 
        // when m_wait_limit is zero
        m_cv_semaphore.notify_one();
    }
    m_cv_vector.wait(lock, [&]() { return !m_vector.empty() || m_state_stop; });
    m_semaphore--;
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
    m_cv_vector.notify_all();
    m_cv_semaphore.notify_all();
}

template <typename T>
unsigned ParallelVector<T>
    ::wake_on_empty_n_waiting(unsigned t_waiting_threads)
    {
        unique_lock<mutex> lock(m_mutex);
        m_wait_limit = t_waiting_threads;
        m_cv_semaphore.wait(lock, 
            [&]() { return (m_vector.empty() 
                && t_waiting_threads <= m_semaphore) 
                    || m_state_stop; });
        m_wait_limit = 0;
        return m_semaphore;
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
    m_cv_vector.notify_one();
}

template <typename T>
ParallelVector<T>::~ParallelVector() 
{
    stop_vector();
}