#ifndef FILE_READ_H
#define FILE_READ_H

/*

                    ?

*/

#include <iostream>
#include <string>
#include "task_parallelizer.h"
#include <filesystem>

namespace fs = filesystem;
using namespace std;

// todo struct na offset-char*
// bude 2,3? arg templatu?

class FileRead : public TaskParallelizer<fs::path, char*, string, Terminator>{
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