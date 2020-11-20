#ifndef DISK_ITERATE_H
#define DISK_ITERATE_H

/*

                    ?

*/

#include <iostream>
#include <string>
#include "task_parallelizer.h"
#include <filesystem>
#include "file_read.h"

namespace fs = filesystem;

using namespace std;

class FileIterate : public TaskParallelizer<string, fs::path, string, FileRead>{
    public:
        FileIterate(const struct job_details t_jobs[], 
            const unsigned t_job_num, 
            TaskContainer* t_super_job_class = nullptr);
        void start(const string &t_path);
        ~FileIterate();

    protected:
        virtual void start();
        void process_sub_results();
};

#endif