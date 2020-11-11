#ifndef TASKPARALELIZER_H
#define TASKPARALELIZER_H

/*

                TODO
        pozriet:
         exceptions, noexcept
        pozriet co sa da:
            skonstantnit
            zosmernikovat (menej kopirovani)
            zostaticknit
            inlinovat
        doplnit:
            potrebne obj-oriented "povinne" funkcie
*/

#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <exception>

using namespace std;

// Exception used by next_job_argument function:
class NoMoreJob : public runtime_error {};

// Template struct for passing job description 
// details for TaskParallelizer class(es)
template <typename T>
struct job_details {
    const T job_detail;
    const unsigned int thread_number;
    const unsigned int job_segment_size;
};

// TaskParallelizer class
// typename S (input type)
//  is type which is processed by TaskParallelizer
// typename T (output type)
//  is type of output of TaskParallelizer
template <typename S, typename T>
class TaskParallelizer {

    private:
        const bool m_parallel;
        vector<T> m_next_job;
        vector<TaskParallelizer*> m_sub_job_class;
        vector<thread*> m_threads;
        struct job_details m_job_details[];
        const unsigned int m_job_details_num;      
        TaskParallelizer* m_super_job_class;
        mutex m_mutex;
        condition_variable m_cond_var;
        bool m_job_finish = false;

    public:
        TaskParallelizer(const struct job_details t_jobs[], 
            const unsigned int t_job_num, 
            TaskParallelizer* t_super_job_class = nullptr);

        inline void start();
        void start(const S &t_single_job);

        const T next_job_argument();

        ~TaskParallelizer();

    private:
        // Function do_job implements main functionality (does job) 
        // which can be parallelised by TaskParallelizer functionality
        virtual void do_job(const S &t_single_job) = 0;

        void call_sub_job(const T &t_item);

        inline void check_and_set_finished();

    protected:
        // Method for filling m_sub_job_class vector.
        // Use new: m_sub_job_class.push_back(new ...). 
        //  TaskParellelizer destructor uses delete.
        // Vector can be empty, but then you can't call call_sub_job.
        // argument: 
        //  - t_thread_num: number of threads used in this instance
        virtual inline void assign_sub_job_class(const unsigned int t_thread_num) = 0;
};

#endif