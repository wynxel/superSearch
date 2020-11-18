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
#include <type_traits>
#include "util/parallel_vector.h"

using namespace std;

// Exception used by next_job_argument function:
// TODO pojde prec
class NoMoreJob : public exception {
    public:
        NoMoreJob() : exception(){};
};

// Settings for TaskParallelizer instance:
typedef struct job_details {
    const void* job_detail;
    const unsigned thread_number;
    const unsigned job_segment_size;
} job_details;

// Super class for TaskParallelizer.
// Declares two functions, that needs to be 
// called on TaskParallelizer instance
// even if we don't know template types.
class TaskContainer{
    public:
        virtual void start() = 0;
        virtual void start_parallel_cycle() = 0;
        virtual ~TaskContainer() {};
};

// Class for closing TaskParallelizer derived classes
// chain. TaskParallelizer has vector m_sub_job_class
// and this vector must be non empty even when it will
// be never used. So for such cases, use this class:
class Terminator : public TaskContainer {
    public: 
        Terminator(const job_details t_jobs[], 
            const unsigned t_job_num, 
            TaskContainer* t_super_job_class = nullptr){};
        void start() {};
        void start_parallel_cycle() {};

};

// TaskParallelizer class
// typename S (job input type)
//  or super-job class output type
// typename T (job output type)
//  or sub-job class input type
// Scheme for S and T:
//  S -> do job -> U
//  S is obtained from super-job class
//  U is passed to sub-job class
// typename U (main output type (or result))
//  is type of result, which is produced 
//  by this class after finishing all job
//  and all sub-jobs, which were
//  started by this class.
// class C is class processing sub jobs
//  must be derived from TaskParallelizer. 
//  if there will be no more sub jobs, use
//  Terminator class.

//template <typename S, typename T, class C, 
//    class = typename enable_if<is_base_of<TaskContainer, C>::value>::type>
//typename enable_if<is_base_of<TaskParallelizer, C>::value, C>::type
template <typename S, typename T, typename U, class C>
class TaskParallelizer : public TaskContainer{

    protected:
        const bool m_parallel;
        const job_details* m_job_details;    

    private:
        const unsigned m_job_details_num;
        ParallelVector<T> m_next_job;
        ParallelVector<U> m_job_result;
        vector<TaskContainer*> m_sub_job_class;
        vector<thread*> m_threads;
        TaskContainer* m_super_job_class;
        const T* m_single_thread_arg_shortcut;
        bool m_job_finish;

    public:
        TaskParallelizer(const job_details t_jobs[], 
            const unsigned t_job_num, 
            TaskContainer* t_super_job_class = nullptr);
        virtual void start(const S &t_single_job) = 0;
        bool is_parallel();
        const T next_job_argument();
        bool get_result_num();
        U get_result_blocking();
        U get_result_non_blocking();
        ~TaskParallelizer();

    private:
        // Start function only gets argument and calls start(argument)
        // This function should contain only this, and definte TYPE:
        /*
            TaskParallelizer<RIGHT TEMPLATE>* super_class = 
                (TaskParallelizer<RIGHT TEMPLATE>*) get_super_class();
            if (super_class == nullptr) {
                start(next_job_argument());
            } else {
                start(super_class->next_job_argument());
            }
        */
        // (maybe this function could be implemented by some metafunction)
        virtual void start() = 0;
        void start_parallel_cycle();

    protected:
        void call_sub_job(const T &t_item);
        inline TaskContainer* get_super_class();
        inline void notify_sub_to_finish();
};
#include "task_parallelizer.cpp"
#endif