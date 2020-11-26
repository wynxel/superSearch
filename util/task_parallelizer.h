#ifndef TASKPARALELIZER_H
#define TASKPARALELIZER_H

/*
    TaskParallelizer class is an abstract class,
    which helps to parallelize task, which can be
    written in the way of nested for cycles. For example:
        for (drive : all_drives){
            for (file : drive) {
                for (data_chunk : file){
                    do something with data_chunk...
                }
            }
        }
    If program can be written in this way, then one instance
    of TaskParallelizer can represent one for loop. 
    Body of this for loop can be "placed" into start() method
    of TaskParallelizer derived class (TaskParallelizer is 
    abstract class...). Then, when you will for example 
    start instance of this derived class with setting number of
    threads to 4, total work of this "for cycle" 
    will be split between 4 threads. This is simplified explanation
    of TaskParallelizer. If you want to use it, please read
    comments of TaskParallelizer functions in this headder file (
        especially functions: start(), call_sub_job(), 
        process_sub_results()..)
    Key functionality of TaskParallelizer is in underlaying stack
    (parallel_vector), which synchronizes produce-consume requests
    between those nested for cycles, which are being parallelized
    by this awesome class. 
*/

#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <exception>
#include <type_traits>
#include "parallel_vector.h"
#include "tp_const.h"

using namespace std;

// Exception used by next_job_argument function:
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
        Terminator(const job_details t_jobs[], const unsigned t_job_num, 
            TaskContainer* t_super_job_class = nullptr, 
            const int t_id = tpconst::CLASS_ID_DEF){};
        void start() {};
        void start_parallel_cycle() {};
};

// TaskParallelizer class
// typename IN (job input type)
//  or super-job class output type
// typename OUT (job output type)
//  or sub-job class input type
// Scheme for IN and OUT:
//  IN -> do job -> BCK
//  IN is obtained from super-job class
//  BCK is passed to sub-job class
// typename BCK (main output type (or result))
//  is type of result, which is produced 
//  by this class after finishing all job
//  and all sub-jobs, which were
//  started by this class.
// class SBJB is class processing sub jobs
//  must be derived from TaskParallelizer. 
//  if there will be no more sub jobs, use
//  Terminator class.

// TODO: force SBJB to be subclass of TaskParallelizer:
//template <typename IN, typename OUT, class SBJB, 
//    class = typename enable_if<is_base_of<TaskContainer, SBJB>::value>::type>
//typename enable_if<is_base_of<TaskParallelizer, SBJB>::value, SBJB>::type
template <typename IN, typename OUT, typename BCK, class SBJB>
class TaskParallelizer : public TaskContainer{

    protected:
        const int m_class_id;
        const bool m_parallel;
        const job_details* m_job_details;    
        ParallelStack<BCK> m_sub_job_results;

    private:
        const unsigned m_job_details_num;
        ParallelStack<OUT> m_next_job;
        vector<TaskContainer*> m_sub_job_class;
        vector<thread*> m_threads;
        TaskContainer* m_super_job_class;
        OUT m_single_thread_arg_shortcut;
        bool m_shortcut_enable;
        bool m_job_finish;

    public:
        TaskParallelizer(const job_details t_jobs[], 
            const unsigned t_job_num, 
            TaskContainer* t_super_job_class = nullptr,
            const int t_id = tpconst::CLASS_ID_DEF);
        /* 
            Main function: void start(IN &t_single_job)
            This function should do it's job according to:
                1) t_single_job parameter
                2) m_job_details[0] informations
            Result, of this job should be passed to
            call_sub_job(OUT &t_item) function (which then
            pass this result to sub-job class(es)).
            This function should then process result from sub-job
            classes by calling process_sub_results().
        */
        virtual void start(IN &t_single_job) = 0;
        inline bool is_parallel() noexcept;
        OUT next_job_argument();
        inline void put_sub_result(BCK &t_result);
        ~TaskParallelizer();

    private:
        /*
            Start function only gets argument and calls start(argument)
            This function should contain only this code, with right template:
                {
                TaskParallelizer<RIGHT TEMPLATE>* super_class = 
                    (TaskParallelizer<RIGHT TEMPLATE>*) get_super_class();
                start(super_class->next_job_argument());
                }
            (maybe this function could be implemented by some metafunction)
            This function is invoked only within TaskParallelizer in two cases:
                1) in multithread loop
                2) in sub-job single thread mode, when invoking sub-job.start()
            start() can be sure, that get_super_class() won't return nullptr
                in case 2) there is super class
                in case 1) nullptr is checked in start_parallel_cycle()
        */
        virtual void start() = 0;
        void start_parallel_cycle();

    protected:
        inline void wait_to_sub_finish() noexcept;
        inline void call_sub_job(OUT t_item);
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
            Note2: if sub-job threads are pushing results created
            by calling new, don't forget to free memory in this function!
        */
        virtual void process_sub_results() = 0;
};


#include "task_parallelizer.cpp"
#endif