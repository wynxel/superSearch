#ifndef DISK_ITERATE_H
#define DISK_ITERATE_H

/*
    Start method of this class iterates all files in 
    folder, which was passed to this class.
    Files are passed to FileRead class, which is responsible
    for reading each file. 
*/

#include <iostream>
#include <string>
#include <filesystem>
#include "util/task_parallelizer.h"
#include "file_read.h"

namespace fs = filesystem;

using namespace std;

class FileIterate : public TaskParallelizer<string, fs::path, int, FileRead>{
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