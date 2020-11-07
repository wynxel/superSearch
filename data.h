#ifndef DATA_H
#define DATA_H

/*

                TODO

*/

#include <filesystem>
#include <string>

using namespace std;

// struct for storing program arguments:
struct prog_data{
    const string to_find;
    const filesystem::path path;
    const int t1;
    const int t2;
    const int b2_buf;
};

struct job_details {
    const string job_argument;
    const int thread_number;
    const int job_segment_size;
};


#endif