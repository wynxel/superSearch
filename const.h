#ifndef CONST_H
#define CONST_H

/*

                TODO

*/

#include <string>
#include <boost/format.hpp>

using namespace std;

namespace progconst{
    // defaults:
    const unsigned THR_MIN = 0;
    const unsigned THR_MAX = 10;
    const unsigned BUF_NDEF = 0;
    const unsigned BUF_MIN = 1;
    const unsigned BUF_MAX = 10000;
    const unsigned BUF_MULT = 1000;
    const unsigned BUF_DEF = 1000;
    const unsigned IBUF_MIN = 512;
    const unsigned IBUF_DEF = 6000000;
    const unsigned IBUF_MAX = 500000000;

    // switches: 
    const string arg_t1 = "-t1";
    const string arg_t2 = "-t2";
    const string arg_in_buf = "-ib";
    const string arg_t2_buf = "-buf";

    // manual: 
    const string manual = boost::str(boost::format{"Ussage: search PATH PATTERN [OPTIONS]\nOptions:"
        "\n\t-t1  NUM\t process NUM files in parallel"
        "\n\t\t\t min: %1%, max: %2%, single thread (default): %1%"
        "\n\t-t2  NUM\t run search in each file with max NUM threads"
        "\n\t\t\t min: %3%, max: %4%, single thread (default): %3%"
        "\n\t-buf NUM\t if -t2 > 1, split file to segments of size NUM and search in segments in parallel"
        "\n\t\t\t min: %5%, max: %6% (X %7% bites, default: %8%)"
        "\n\t-ib  NUM\t size of file reading buffer. Best option is to set it to L3 cache size."
        "\n\t\t\t min: %9%, max: %10% (default: %11%)"} 
        % THR_MIN % THR_MAX % THR_MIN % THR_MAX % BUF_MIN % BUF_MAX % BUF_MULT 
        % BUF_DEF % IBUF_MIN % IBUF_MAX % IBUF_DEF);

    // argument errors:
    const string arg_few = "Error: not enough arguments.";
    const string not_number = "Error: expected number, got: ";
    const string not_switch = "Error: unknown switch: ";
    const string wrong_path = "Error: provided path does not exists: ";
    const string wrong_value_t1 = boost::str(boost::format{"Error: use min %1% and max %2% threads for -t1."} 
        % THR_MIN % THR_MAX);
    const string wrong_value_t2 = boost::str(boost::format{"Error: use min %1% and max %2% threads for -t2."} 
        % THR_MIN % THR_MAX);
    const string wrong_value_buf = boost::str(boost::format{"Error: use min %1% and max %2% for segment buffer size."} 
        % BUF_MIN % BUF_MAX);
    const string wrong_value_ibuf = boost::str(boost::format{"Error: use min %1% and max %2% for reading buffer size."} 
        % IBUF_MIN % IBUF_MAX);

    // program errors:
    const string file_error = "Unexpected error: ";
    const string file_size_error = "Unable to determine file size: ";
    const string file_open_error = "Can't open file: ";
    const string segment_vs_buffer = "Segment size is bigger than read buffer size.";
    const string warn_thread_vs_segment = "Read buffer size offers not enought segments for threads. "
        "Try to change ratio of reading buffer size, segment size and number of threads.";


    // runtime exceptions:
}

#endif