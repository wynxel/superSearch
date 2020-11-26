#ifndef TP_CONST_H
#define  TP_CONST_H

/*
    TaskParallelizer constants and strings
*/

#include <string>

using namespace std;

namespace tpconst{
    // const:
    const unsigned NO_THREAD = 0;
    const unsigned THREAD_MAX = 20;
    const unsigned NO_SEG = 0;
    const unsigned SEG_MAX = 1500000;
    const unsigned CLASS_ID_LIMIT = 100;
    const int CLASS_ID_DEF = -1;


    // runtime exceptions:
    const string no_multithread = "This function can be called only in multithread mode.";
    const string no_job_to_do = "Wrong use of start() function (neither multithread mode nor start() with argument).";
    const string too_many_threads = "More threads than allowed.";
    const string wrong_seg_size = "Too big segment size.";
    const string thread_vs_class_count = "Wrong m_sub_job_class initialization. Size of m_sub_job_class "
        "vector is not equal to number of threads.";
    const string no_sub_job_class = "Function call_sub_job(...) can't be used if no sub_job classes are set-up. "
        "See assign_sub_job_class(...) function.";
    const string class_illegal_use = "Class was constructed withou parameters. Use parametrizied constructor "
        "or call setup(...).";
    const string job_num_len = "Argument t_job_num is 0. TaskParallelizer must get job_details argument.";
}


#endif