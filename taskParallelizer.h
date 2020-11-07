#ifndef TASKPARALELIZER_H
#define TASKPARALELIZER_H

/*

                TODO
        pozriet co sa da:
            skonstantnit
            zosmernikovat (menej kopirovani)
            zostaticknit
            inlinovat
        doplnit:
            potrebne obj-oriented "povinne" funkcie
*/

#include "data.h"
#include <iostream>
#include <vector>
#include <mutex>
#include <thread>

using namespace std;
template <typename S, typename T>
//typename 

class TaskParallelizer {
    /*  POTREBUJEME ESTE:
        pole threadov
    */

    // variables:
    private:
        const bool m_paralell;
        vector<const T> m_data;
        //vector<const T*> m_data_done; // delete in destructor
        TaskParallelizer** m_sub_job_class;
        TaskParallelizer* m_super_job_class;
        mutex m_mutex;

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
        TaskParallelizer(const struct job_details t_my_job,
            const struct job_details t_last_job,
            TaskParallelizer* t_super_job_class = nullptr);
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
        virtual void do_job(const S t_single_job = nullptr) = 0;
        void call_sub_job(const T t_item) = 0;

    protected:
        inline void assign_sub_job_class();

};

#endif