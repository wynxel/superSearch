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

class FileRead : public TaskParallelizer<fs::path, segment*, result*, StringSearch>{
    public:
        FileRead(const struct job_details t_jobs[], const unsigned t_job_num, 
            TaskContainer* t_super_job_class = nullptr, const int t_id = -1);
        virtual void start(fs::path &t_path);
        ~FileRead(); // DELETE OUT SEGMENTS!

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