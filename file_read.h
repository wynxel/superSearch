#ifndef FILE_READ_H
#define FILE_READ_H

/*

                    ?

*/

#include <iostream>
#include <string>
#include <filesystem>
#include "util/task_parallelizer.h"
#include "string_search.h"

namespace fs = filesystem;
using namespace std;

class FileRead : public TaskParallelizer<fs::path, segment, segment, StringSearch>{
    public:
        FileRead(const struct job_details t_jobs[], 
            const unsigned t_job_num, 
            TaskContainer* t_super_job_class = nullptr);
        ~FileRead();

    protected:
        virtual void start();
        virtual void start(const fs::path &t_path);
        void process_sub_results();
};

#endif