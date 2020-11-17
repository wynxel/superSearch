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

using namespace std;

// Exception used by next_job_argument function:
class NoMoreJob : public exception {
    public:
        NoMoreJob() : exception(){};
};

// Settings for TaskParallelizer instance
struct job_details {
    const void* job_detail;
    const unsigned thread_number;
    const unsigned job_segment_size;
};

// Super class for TaskParallelizer.
// Declares two functions, that needs to be 
// called on TaskParallelizer instance
// even if we don't know template types
class TaskContainer{
    public:
        virtual void start() = 0;
        virtual void start_parallel() = 0;
        virtual ~TaskContainer() {};
};

class Terminator : public TaskContainer {
    public: 
        Terminator(const struct job_details t_jobs[], 
            const unsigned t_job_num, 
            TaskContainer* t_super_job_class = nullptr){};
        void start() {};
        void start_parallel() {};

};

// TaskParallelizer class
// typename S (input type)
//  is type which is processed by TaskParallelizer
// typename T (output type)
//  is type of output of TaskParallelizer

// podmienka na to, co je C!
template <typename S, typename T, class C>
//typename enable_if<is_base_of<TaskParallelizer, C>::value, C>::type
class TaskParallelizer : public TaskContainer{

    private:
        const bool m_parallel;
        const unsigned m_job_details_num;

    protected:
        vector<T> m_next_job;
        vector<TaskContainer*> m_sub_job_class;
        vector<thread*> m_threads;
        const struct job_details* m_job_details;    
        TaskContainer* m_super_job_class;
        mutex m_mutex;
        condition_variable m_cond_var;
        bool m_job_finish;
        const T* m_single_thread_arg_shortcut;

    public:
        TaskParallelizer(const struct job_details t_jobs[], 
            const unsigned t_job_num, 
            TaskContainer* t_super_job_class = nullptr);

        // Start function only gets argument and calls start(argument)
        // This function should contain only this, and definte TYPE:
        /*
            if (m_super_job_class == nullptr) {
                start(next_job_argument());
            } else {
                start(((TaskParallelizer<TYPE, TYPE>*)m_super_job_class)->next_job_argument());
            }
        */
        // As template types are statically check during compile time
        // this function can't be implemented in TaskParallelizer and must
        // be implemented in class which extends TaskParallelizer.
        virtual void start() = 0;

        // viac veci urobit protected?
        void start_parallel();
        virtual void start(const S &t_single_job) = 0;
        bool is_parallel();

        const T next_job_argument();
        ~TaskParallelizer();

    private:

    protected:
        void call_sub_job(const T &t_item);

        inline void check_and_set_finished();

};
#include "task_parallelizer.cpp"


#endif