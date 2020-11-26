#ifndef STRING_SEARCH_H
#define STRING_SEARCH_H

/*
    Start function of StringSearch takes segment from
    super-job class (FileRead) and search for pattern in
    this segment. If match, details of this match are
    stored in struct result and pushed to super-job class.
    This class does not have any more sub jobs.
*/

#include <iostream>
#include <string>
#include <cstring>
#include <filesystem>
#include "util/task_parallelizer.h"
#include <algorithm>
#include "const.h"

namespace fs = filesystem;

// struct, which contains details
// for StringSearch and search function:
typedef struct segment{
    // this segment is in "offset" position in 
    // the file
    const unsigned offset;
    // is this segment at the very end of file?
    const bool last;
    // length of this segment
    const unsigned length;
    // pointer to char array
    // (use data[0]...data[length-1])
    char* data;
} segment;

// result of search reported to FileRead class:
typedef struct result{
    const char* prefix;
    const char* sufix;
    const unsigned offset;

    // delete prefix, sufix, as they are created
    // by new char[]
    ~result(){
        if (prefix != nullptr) {
            delete[] prefix;
        }
        if (sufix != nullptr) {
            delete[] sufix;
        }
    }

    // compare operators, structs can be sorted
    // by offset number
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

    private:
        char* create_escaped_substring(const char* from, const unsigned length);
};

#endif