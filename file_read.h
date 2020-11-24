#ifndef FILE_READ_H
#define FILE_READ_H

/*

                    ?

*/

#include <iostream>
#include <string>
#include <filesystem>
#include <cstdio>
#include "util/task_parallelizer.h"
#include "string_search.h"
#include "const.h"

namespace fs = filesystem;
using namespace std;

class FileRead : public TaskParallelizer<fs::path, segment*, segment*, StringSearch>{
    public:
        FileRead(const struct job_details t_jobs[], 
            const unsigned t_job_num, 
            TaskContainer* t_super_job_class = nullptr);
        virtual void start(fs::path &t_path);
        ~FileRead(); // DELETE OUT SEGMENTS!

    protected:
        virtual void start();
        void process_sub_results();

    private:
        unsigned m_buffer_len;
        unsigned m_seg_len;
        char* m_buffer;
        vector<segment*> garbage;
};

#endif