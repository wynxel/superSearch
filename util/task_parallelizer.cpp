/*
    see task_parallelizer.h
*/

#include "task_parallelizer.h"

// Regular constructor:
// Arguments:
//  - t_jobs: array of jobs (t_jobs[0] is for this class, 
//      t_jobs[1] is for sub_job_class and so on...)
//  - t_job_num: length of t_jobs
//  - t_super_job_class: pointer to super class (if exists)
//  - t_id: optional, just id of class, usefull for debugging
// Class TaskParallelizer is abstract, it has pure virtual methods. 
// Exceptions:
//  throws invalid_argument with string message with details
template <typename IN, typename OUT, typename BCK, class SBJB>
TaskParallelizer<IN, OUT, BCK, SBJB>::TaskParallelizer(const job_details t_jobs[], 
    const unsigned t_job_num, TaskContainer* t_super_job_class,
    const int t_id) :
    m_class_id(t_id),
    m_parallel(t_jobs[0].thread_number > tpconst::NO_THREAD ? true : false),
    m_job_details(t_jobs),
    m_job_details_num(t_job_num),
    m_super_job_class(t_super_job_class),
    m_shortcut_enable(false),
    m_job_finish(false)
    {
        // check: thread number, segment size, job details length:
        const unsigned thread_num = t_jobs[0].thread_number;
        if (thread_num > tpconst::THREAD_MAX) {
            throw invalid_argument(tpconst::too_many_threads);
        }

        if (m_parallel && t_jobs[0].job_segment_size > tpconst::SEG_MAX) {
            throw invalid_argument(tpconst::wrong_seg_size);
        }

        if (t_job_num < 1) {
            throw invalid_argument(tpconst::job_num_len);
        }

        // set-up sub-job classes and threads
        // (assert: m_sub_job_class can't be empty)
        // or set up onlu one sub-job class:
        if (m_parallel) {
            for (unsigned i = 0; i < thread_num; i++) {
                const int class_id = 
                    t_id == tpconst::CLASS_ID_DEF ? 
                    i : (i + tpconst::CLASS_ID_LIMIT * t_id);
                SBJB* sub_job_class = new SBJB(
                    t_jobs + 1, t_job_num - 1, this, class_id);
                thread* job_thread = new thread(&TaskContainer::start_parallel_cycle, 
                    sub_job_class);
                // add to vector:
                m_sub_job_class.push_back(sub_job_class);                
                m_threads.push_back(job_thread);
            }
        } else {
            m_sub_job_class.push_back(new SBJB(t_jobs + 1, t_job_num - 1, this));
        }
    }

// Use only in multithread mode (otherwise 
// logical_error exception).
// Function returns OUT value, which represents
// next job to be done.
// If super-job thread is still working, but queue
// with jobs is currently empty, thread calling this
// function will go to waiting state untill super-job
// class makes new job OUT value.
// Exception:
//  If super-job class finished, this function will
//  throw "NoMoreJob" exception indicating, that theread,
//  which called this function should finish. 
template <typename IN, typename OUT, typename BCK, class SBJB>
OUT TaskParallelizer<IN, OUT, BCK, SBJB>::next_job_argument()
{
    if (!m_parallel) {
        if (! m_shortcut_enable) {
            throw NoMoreJob();
        }
        m_shortcut_enable = false;
        return m_single_thread_arg_shortcut;
    } else {
        try {
            return m_next_job.pop_blocking();
        } catch (Empty& e) {
            throw NoMoreJob();
        }
    }
}

// Main function used in multithread mode.
// Checks super-job pointer and starts cycle
// with start function. Breaks, if there is
// no more job from super class.
// exception: runtime_error, if super class is null
template <typename IN, typename OUT, typename BCK, class SBJB>
void TaskParallelizer<IN, OUT, BCK, SBJB>::start_parallel_cycle()
{
    if (m_super_job_class == nullptr) {
        // in multithread mode, start() function will
        // ask super job_class for job argument. If means,
        // that m_super_job_class can't be nullptr
        throw runtime_error(tpconst::no_job_to_do);
    }
    while (true) {
        try {
            start();
        } catch (NoMoreJob& except) {
            break; 
        }
    }
}

// Function turns on controll flag variable
// that indicates, that running threads should
// finish their job and exit. 
// Function then notifies all sleeping threads.
// Note: after this call, calling call_sub_job will be 
// throwing exception.
template <typename IN, typename OUT, typename BCK, class SBJB>
void TaskParallelizer<IN, OUT, BCK, SBJB>::notify_sub_to_finish()
noexcept {
    if (! m_parallel) {
        return;
    } else {
        m_next_job.stop_vector();
    }
}

// This function passes OUT value representing 
// job to be done to next class which should 
// process it. In multithread mode: only add to
// queue, in single thread: call directly 
// sub-job function.
// Exception:
//  logic_error - if m_next_job stack is stopped
//  (this class stops m_next_job stack, only in
//  class destructor)
template <typename IN, typename OUT, typename BCK, class SBJB>
inline void TaskParallelizer<IN, OUT, BCK, SBJB>::call_sub_job(OUT t_item)
{
    if (m_parallel) {
        m_next_job.push(t_item);
    } else {
        m_single_thread_arg_shortcut = t_item;
        m_shortcut_enable = true;
        m_sub_job_class[0]->start();
    }
}

// function fo sub-job threads. Calling this function will push
// return value (BCK result) created by sub-job thread to super-job
// class/thread. 
// exception: thwos logic_error if stack is already stopped (that means,
// that no more return values can be pushed)
template <typename IN, typename OUT, typename BCK, class SBJB>
inline void TaskParallelizer<IN, OUT, BCK, SBJB>::put_sub_result(BCK &t_result)
{
    m_sub_job_results.push(t_result);
}

// Blocks thread which calls this function untill 
// all its sub-job threads are sleeping because of
// empty vector.
template <typename IN, typename OUT, typename BCK, class SBJB>
void TaskParallelizer<IN, OUT, BCK, SBJB>::wait_to_sub_finish()
noexcept {
    m_next_job.wake_on_empty_n_waiting
        (m_job_details[0].thread_number);
}

// get pointer to super class (super-job)
// returns nullptr if no super class
template <typename IN, typename OUT, typename BCK, class SBJB>
TaskContainer* TaskParallelizer<IN, OUT, BCK, SBJB>::get_super_class()
noexcept {
    return m_super_job_class;
}

// Return true if multithread mode
template <typename IN, typename OUT, typename BCK, class SBJB>
inline bool TaskParallelizer<IN, OUT, BCK, SBJB>::is_parallel()
noexcept {
    return m_parallel;
}

// Destructor
template <typename IN, typename OUT, typename BCK, class SBJB>
TaskParallelizer<IN, OUT, BCK, SBJB>::~TaskParallelizer()
{
    // chceck if finished flag is set:
    notify_sub_to_finish();

    // this should be not necessary:
    m_sub_job_results.stop_vector();

    // wait for threads:
    for (auto& elem_thread : m_threads) {
        elem_thread->join();
    }

    // process results from sub-job classes, if any left:
    // (as process_sub_results is pure virtual, can't call
    // it here. Although it would be nice to preventively
    // call it in this destructor)
    // process_sub_results();
    
    // delete threads:
    for (auto& elem_thread : m_threads) {
        delete elem_thread;
    }

    // delete m_sub_job_class:
    for (auto& elem_class : m_sub_job_class) {
        delete elem_class;
    }
}