#ifndef DISK_ITERATE_H
#define DISK_ITERATE_H

/*

                    ?

*/

#include <iostream>
#include <string>
#include "task_parallelizer.h"
#include <filesystem>

namespace fs = filesystem;

using namespace std;

class FileIterate : public TaskParallelizer<string, fs::path>{
    public:
        FileIterate(const struct job_details t_jobs[], 
            const unsigned t_job_num, 
            TaskParallelizer* t_super_job_class = nullptr);
        FileIterate();
        virtual void start(const string &t_path);
        ~FileIterate();

    protected:
        virtual inline void assign_sub_job_class(const unsigned t_thread_num);
        virtual void start();
};

#endif