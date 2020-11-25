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
    const unsigned length;
    char* data;
} segment;

typedef struct result{
    const char* prefix;
    const char* sufix;
    const unsigned offset;

    bool operator >(const result& other) const {
        return offset > other.offset;
    }
    bool operator <(const result& other) const {
        return offset < other.offset;
    }
    bool operator ==(const result& other) const {
        return offset == other.offset;
    }
    bool operator >=(const result& other) const {
        return offset >= other.offset;
    }
    bool operator <=(const result& other) const {
        return offset <= other.offset;
    }
} result;

using namespace std;

class StringSearch : public TaskParallelizer<segment*, int, int, Terminator>{
    public:
        StringSearch(const struct job_details t_jobs[], const unsigned t_job_num, 
            TaskContainer* t_super_job_class = nullptr, const int t_id = -1);
        void start(segment* &t_path); 
        ~StringSearch();

    protected:
        virtual void start();
        void process_sub_results();
};

#endif