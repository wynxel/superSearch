#include "parallel_vector.h"

// empty constructor:
template <typename T>
ParallelStack<T*>::ParallelStack() : 
    m_state_stop(false),
    m_semaphore(0),
    m_wait_limit(0) {};

// thread unsafe pop private metohd
// used internally by ParallelStack methods
// exception: throws Empty if empty stack
template <typename T>
inline T* ParallelStack<T*>::pop_without_mutex()
{
    if (m_vector.empty())
    {
        throw Empty();
    }

    T* ret_val = m_vector.back();
    m_vector.pop_back();
    return ret_val;
}

// pop from stack
// if empty, thread calling this function will 
// be paused, and will wait for next element
// pushed to stack. 
// exception: throws Empty, when ParallelStack is
// in stop state. That means, that no more elements
// can be pushed and popped from stack.
template <typename T>
T* ParallelStack<T*>::pop_blocking()
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

// pop from stack, wihtout waiting if empty
// exception: throws Empty if empty
template <typename T>
T* ParallelStack<T*>::pop_non_blocking()
{
    unique_lock<mutex> lock(m_mutex);
    return pop_without_mutex();
}

template <typename T>
unsigned ParallelStack<T*>::size() noexcept
{
    unique_lock<mutex> lock(m_mutex);
    return m_vector.size();
}

template <typename T>
bool ParallelStack<T*>::empty() noexcept
{
    unique_lock<mutex> lock(m_mutex);
    return m_vector.empty();
}

template <typename T>
bool ParallelStack<T*>::ptr_compare(const T* elem1, const T* elem2)
{
    return (*elem1 > *elem2);
}

template <typename T>
void ParallelStack<T*>::sort()
{
    unique_lock<mutex> lock(m_mutex);
    if (m_vector.size() > 1) {
        std::sort(m_vector.begin(), m_vector.end(), ptr_compare);
    }
}

// set vector to stop state
// In stop state, push method will throw
// exception and pop function can be called
// until stack is empty. Then pop throws
// exception. This function can be usefull
// when thread pushing elements want's to 
// notify popping threads, that there will be no
// more elements. Calling this function will
// notify threads blocked on pop-wait condition. 
template <typename T>
void ParallelStack<T*>::stop_vector() noexcept
{
    {
        unique_lock<mutex> lock(m_mutex);
        m_state_stop = true;
    }
    m_cv_vector.notify_all();
    m_cv_semaphore.notify_all();
}

// This function blocks calling thread untill:
//  1) stack is empty
//  2) there are t_waiting_threads or more threads
//      blocked on pop waiting condition (empty stack)
// If this conditions are satisfied, this function
// unblocks calling thread and returns number of threads
// waiting on pop-waiting condition (on empty stack condition).
// This function is usefull, when pushing thread want's to wait
// until all popping threads finishes their work and are again 
// waiting for new elements in stack.
// If ParallelStack in stop state, this function may return
// less than t_waiting_threads.
// If another thread is pushing elements to stack meanwhile 
// this thread waits in this function, there is no guarantee,
// that if conditions 1-2 are meet, calling thread will be resumed.
template <typename T>
unsigned ParallelStack<T*>
    ::wake_on_empty_n_waiting(unsigned t_waiting_threads)
    noexcept {
        unique_lock<mutex> lock(m_mutex);
        // set internal variable, which is checked in 
        // pop method:
        m_wait_limit = t_waiting_threads;
        m_cv_semaphore.wait(lock, 
            [&]() { return (m_vector.empty() 
                && t_waiting_threads <= m_semaphore) 
                    || m_state_stop; });
        // reset back to zero:
        m_wait_limit = 0;
        return m_semaphore;
    }

// push to stack, pointer version
// exception: if stack is in stop state, thwos logic_error
template <typename T>
void ParallelStack<T*>::push(T* t_item)
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

// destructor (stops vector, if not stopped)
template <typename T>
ParallelStack<T*>::~ParallelStack() 
{
    stop_vector();
}