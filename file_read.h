#ifndef FILE_READ_H
#define FILE_READ_H

/*
    Start function of this class takes file name and
    reads this file. Characters from file are split to
    "segments" (segment length can be configured) and each
    segment is pushed to TaskParallelizer stack. Then, 
    StringSearch threads are taking this segments from stack
    and performing string search in segments.
    FileRead function also handles programm output, so it
    prints results in process_sub_results() function. 
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

class FileRead : public TaskParallelizer<fs::path, segment*, result*, StringSearch>{
    public:
        FileRead(const struct job_details t_jobs[], const unsigned t_job_num, 
            TaskContainer* t_super_job_class = nullptr, const int t_id = -1);
        virtual void start(fs::path &t_path);
        ~FileRead();

    protected:
        virtual void start();
        void process_sub_results();

    private:
        const unsigned m_sbuf_len;
        const unsigned m_overlap;
        const unsigned m_rbuf_len;
        fs::path m_file_path;
        char* m_rbuf;
        vector<segment*> garb_collect;
        inline void clear_collector();
};

#endif