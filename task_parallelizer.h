#ifndef TASKPARALELIZER_H
#define TASKPARALELIZER_H

/*

                TODO
        dolezite:
            vlakna
            ako si signalizovat, ze uz nebude robota?
            prezriet navratove hodnoty, exceptions, noexcept
        pozriet co sa da:
            skonstantnit
            zosmernikovat (menej kopirovani)
            zostaticknit
            inlinovat
        doplnit:
            potrebne obj-oriented "povinne" funkcie

        problemy:
            co ak nie je multithread ale niekto vola next job arg?

        otazne:
            atomic flag?
*/

//#include "data.h"
#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
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
    // variables:
    private:
        const bool m_parallel;
        vector<T> m_next_job;
        //vector<const T*> m_data_done; // delete in destructor
        vector<TaskParallelizer*> m_sub_job_class;
        vector<thread*> m_threads;
        struct job_details m_job_details[];
        const unsigned int m_job_details_num;      
        TaskParallelizer* m_super_job_class;
        mutex m_mutex;
        condition_variable m_cond_var;
        bool m_job_finish = false;

    public:
        const struct job_details m_my_job;
        const struct job_details m_last_job;

        /* TODO output
            vyrobit triedu, ktora manazuje vystup. Ocakavame ze:
                umoznuje vkladat vystupy (buffrovat)
                umoznuje vypisat (flush)
                mozno viacere buffre, ked do nej pisu viaceri
            musi byt staticka 
        */ 

    // functions:
    public:
        // manage private variables, initialize...
        // run threads
        TaskParallelizer(const struct job_details t_jobs[], 
            const unsigned int t_job_num, 
            TaskParallelizer* t_super_job_class = nullptr);
        void start(const S t_single_job = nullptr);
        const T next_job_argument();
        ~TaskParallelizer();
        

    private:
        // When defining function do_job, function must:
        // - if t_single_job: process only this parameter
        // - else: take parameter from super:
        //      if m_super_job_class not null
        //          throw exception
        //      take parameter: m_super_job_class->next_job_argument()
        // - do main work
        // - call call_sub_job function
        virtual void do_job(const S t_single_job) = 0;
        void call_sub_job(const T t_item) = 0;
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