/*

                TODO
                
    

*/

#include "task_parallelizer.h"
#include "tp_const.h"

// Constructor
// Arguments:
//  - t_jobs: array of jobs (t_jobs[0] is for this class, 
//      t_jobs[1] is for sub_job_class and so on...)
//  - t_job_num: length of t_jobs
//  - t_super_job_class: pointer to super class (if exists)
// Class extending TaskParallelizer must define assign_sub_job_class method.
/*TaskParallelizer(const struct job_details t_jobs[], 
            const unsigned int t_job_num, 
            TaskParallelizer* t_super_job_class = nullptr);
*/
template <typename S, typename T>
TaskParallelizer<S, T>::TaskParallelizer(const struct job_details t_jobs[], 
    const unsigned int t_job_num, TaskParallelizer* t_super_job_class) :
    m_parallel((t_jobs[0].thread_number > tpconst::NO_THREAD) ? true : false),
    m_super_job_class(t_super_job_class)
    {
        // check: thread number, segment size:
        const unsigned int thread_num = t_jobs[0].thread_number;
        if (thread_num > tpconst::THREAD_MAX) {
            throw invalid_argument(tpconst::too_many_threads);
        }
        if (m_parallel && t_jobs[0].job_segment_size > tpconst::SEG_MAX) {
            throw invalid_argument(tpconst::wrong_seg_size);
        }
        
        // job details array:
        m_job_details = t_jobs;
        m_job_details_num = t_job_num;
        
        // set-up sub-job classes:
        assign_sub_job_class(thread_num);

        // check, class-thread count:
        if (m_parallel && 
            m_sub_job_class.size() != thread_num) {
            throw logic_error(tpconst::thread_vs_class_count);
        }

        // start threads:
        if (m_parallel) {
            for (int i = 0; i < thread_num; i++) {
                m_threads.push_back(
                    new thread(&TaskContainer::start, m_sub_job_class[i]));
            }
        }
    }

// Use only in multithread mode (otherwise 
// logical_error exception).
// Function returns T value, which represents
// next job to be done.
// If super-job thread is still working, but queue
// with jobs is currently empty, thread calling this
// function will go to waiting state untill super-job
// class makes new job T value.
// Exception:
//  If super-job class finished, this function will
//  throw "NoMoreJob" exception indicating, that theread,
//  which called this function should finish. 
template <typename S, typename T>
const T TaskParallelizer<S, T>::next_job_argument()
{
    if (!m_parallel) {
        throw runtime_error(tpconst::no_multithread);
    }

    unique_lock<mutex> lock(m_mutex);
    m_cond_var.wait(lock, [&](){return !m_next_job.empty() || m_job_finish;});
    // check for empty and not job_finis, because it may happen, that 
    // main thread add item to m_next_job, then turns job_finish flag and after that
    // this thread wakes up, checks flag and stops
    if (m_next_job.empty()) {
        lock.unlock();
        throw NoMoreJob();
    }

    T ret_val = m_next_job.back();
    m_next_job.pop_back();
    return ret_val;
}

// Do main job using vector of jobs:
template <typename S, typename T>
void TaskParallelizer<S, T>::start()
{
    // check, if super_job_class is initialized:
    if (m_super_job_class == nullptr) {
        throw runtime_error(tpconst::no_job_to_do);
    }
    S* new_job;
    while (true) {
        try {
            new_job = &m_super_job_class->next_job_argument();
        } catch (NoMoreJob except) {
            break; 
        }
        // do_job outside of try block, because we don't want to catch
        // some unhandled, nested NoMoreJob exception from do_job
        do_job(*new_job);
    }
}

// Do main job using direct parameter:
template <typename S, typename T>
inline void TaskParallelizer<S, T>::start(const S &t_single_job)
{
        do_job(t_single_job);   
}


// Function turns on controll flag variable
// that indicates, that running threads should
// finish their job and exit. 
// Function then notifies all sleeping threads.
template <typename S, typename T>
void TaskParallelizer<S, T>::check_and_set_finished()
{
    if (! m_parallel) {
        return;
    } else {
        {
            unique_lock<mutex> lock(m_mutex);
            if (!m_job_finish) {
                return;
            }
            m_job_finish = true;   
        }
        m_cond_var.notify_all();
    }
}

// This function passes T value representing 
// job to be done to next class which should 
// process it. In multithread mode: only add to
// queue, in single thread: call directly function.
// Exception:
//  logic_error - if there is no sub_job class
//  (no sub_job class was declared in constructor)
template <typename S, typename T>
void TaskParallelizer<S, T>::call_sub_job(const T &t_item)
{
    if (m_parallel) {
        {
            unique_lock<mutex> lock(m_mutex);
            m_next_job.push_back(t_item);
        }
        m_cond_var.notify_one();
    } else {
        if (m_sub_job_class.empty()) {
            throw logic_error(tpconst::no_sub_job_class);
        }
        m_sub_job_class[0]->do_job(t_item);
    }
}

// Destructor
template <typename S, typename T>
TaskParallelizer<S, T>::~TaskParallelizer()
{
    // chceck if finished flag is set:
    check_and_set_finished();

    // wait for threads:
    for (auto& elem_thread : m_threads) {
        elem_thread->join();
    }

    // delete threads:
    for (auto& elem_thread : m_threads) {
        delete elem_thread;
    }

    // delete m_sub_job_class:
    for (auto& elem_class : m_sub_job_class) {
        delete elem_class;
    }
}