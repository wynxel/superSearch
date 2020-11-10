#ifndef TP_CONST_H
#define  TP_CONST_H

/*

                TODO

*/

#include <string>

using namespace std;

namespace tpconst{
    // const:
    const int NO_THREAD = 0;
    const int THREAD_MAX = 20;
    const int SEG_MAX = 10000;
    const int SEG_MULT = 1000;

    // runtime exceptions:
    const string no_multithread = "This function can be called only in multithread mode.";
    const string no_job_to_do = "Wrong use of start() function (neither multithread mode nor start() with argument).";
    const string too_many_threads = "More threads than allowed.";
    const string wrong_seg_size = "Too big segment size.";
    const string thread_vs_class_count = "Wrong m_sub_job_class initialization. Size of m_sub_job_class "
        "vector is not equal to number of threads.";
}


#endif