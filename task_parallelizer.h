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
        ParallelStack<U> m_sub_job_results;

    private:
        const unsigned m_job_details_num;
        ParallelStack<T> m_next_job;
        vector<TaskContainer*> m_sub_job_class;
        vector<thread*> m_threads;
        TaskContainer* m_super_job_class;
        const T* m_single_thread_arg_shortcut;
        bool m_job_finish;

    public:
        TaskParallelizer(const job_details t_jobs[], 
            const unsigned t_job_num, 
            TaskContainer* t_super_job_class = nullptr);
        /* 
            Main function: void start(const S &t_single_job)
            This function should do it's job according to:
                1) t_single_job parameter
                2) m_job_details[0] informations
            Result, of this job should be passed to
            call_sub_job(const T &t_item) function (which then
            pass this result to sub-job class(es)).
            This function should then process result from sub-job
            classes by calling process_sub_results().
        */
        virtual void start(const S &t_single_job) = 0;
        bool is_parallel() noexcept;
        const T next_job_argument();
        void put_sub_result(U t_result);
        ~TaskParallelizer();

    private:
        /*
            Start function only gets argument and calls start(argument)
            This function should contain only this, with right template:
            
            TaskParallelizer<RIGHT TEMPLATE>* super_class = 
                (TaskParallelizer<RIGHT TEMPLATE>*) get_super_class();
            if (super_class == nullptr) {
                start(next_job_argument());
            } else {
                start(super_class->next_job_argument());
            }
            (maybe this function could be implemented by some metafunction)
        */
        virtual void start() = 0;
        void start_parallel_cycle();

    protected:
        inline void wait_to_sub_finish() noexcept;
        void call_sub_job(const T &t_item);
        inline TaskContainer* get_super_class() noexcept;
        inline void notify_sub_to_finish() noexcept;
        /*
            This function should take results from sub-job class(es)
            which are placed to m_sub_job_results stack, and then
            do with them what is necessary (print them or 
            pass to super class). 
            Note: if sub-job is done in
            multithread mode, this function should first call 
            wait_to_sub_finish(), to be sure, that all threads pushed
            results of their job to m_sub_job_results.
        */
        virtual void process_sub_results() = 0;
};
#include "task_parallelizer.cpp"
#endif