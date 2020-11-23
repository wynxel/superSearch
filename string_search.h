#ifndef STRING_SEARCH_H
#define STRING_SEARCH_H

/*

                    ?

*/

#include <iostream>
#include <string>
#include <filesystem>
#include "util/task_parallelizer.h"

namespace fs = filesystem;

typedef struct segment{
    const unsigned offset;
    const char* const data;
} segment;

using namespace std;

class StringSearch : public TaskParallelizer<segment*, int, segment*, Terminator>{
    public:
        StringSearch(const struct job_details t_jobs[], 
            const unsigned t_job_num, 
            TaskContainer* t_super_job_class = nullptr);
        void start(segment* &t_path); 
        ~StringSearch();

    protected:
        virtual void start();
        void process_sub_results();
};

#endif