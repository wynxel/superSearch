#ifndef DISK_ITERATE_H
#define DISK_ITERATE_H

/*

                    ?

*/

#include <iostream>
#include <string>
#include "task_parallelizer.h"

using namespace std;

class FileIterate : public TaskParallelizer<string, string>{

    protected:
        virtual inline void assign_sub_job_class(const unsigned int t_thread_num);
        virtual void do_job(const string &t_single_job);
};

#endif