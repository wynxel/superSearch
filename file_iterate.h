#ifndef DISK_ITERATE_H
#define DISK_ITERATE_H

/*

                    ?

*/

#include <iostream>
#include <string>
#include <filesystem>
#include "util/task_parallelizer.h"
#include "file_read.h"

namespace fs = filesystem;

using namespace std;

class FileIterate : public TaskParallelizer<string, fs::path, string, FileRead>{
    public:
        FileIterate(const struct job_details t_jobs[], const unsigned t_job_num, 
            TaskContainer* t_super_job_class = nullptr, const int t_id = -1);
        void start(string &t_path);
        ~FileIterate();

    protected:
        virtual void start();
        void process_sub_results();
};

#endif