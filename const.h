#ifndef CONST_H
#define CONST_H

/*
    constrants and string messages for
    superSearch utility:
*/

#include <string>
#include <boost/format.hpp>

using namespace std;

namespace progconst{
    // defaults:
    // ((!) TaskParallelizer has its own 
    // thread, buffer constants)
    const unsigned THR_MIN = 0;
    const unsigned THR_MAX = 10;
    const unsigned BUF_NDEF = 0;
    const unsigned SBUF_MIN = 128;
    const unsigned SBUF_MAX = 1500000;
    const unsigned SBUF_DEF = 65536;
    const unsigned RBUF_MIN = 512;
    const unsigned RBUF_DEF = 6000000;
    const unsigned RBUF_MAX = 500000000;
    const unsigned SBUF_DEF_RATIO = 2;
    const unsigned RBUF_DEF_RATIO = 10;
    const unsigned PREFIX_LEN = 3;
    const unsigned SUFIX_LEN = 3;
    const bool VERB_DEF = false;



    // switches: 
    const string switch_verb = "-v";
    const string switch_t1 = "-t1";
    const string switch_t2 = "-t2";
    const string switch_rbuf = "-rb";
    const string switch_sbuf = "-sb";
    const string switch_h1 = "-h";

    // manual: 
    const string manual = boost::str(boost::format{"Usage: search PATH PATTERN [OPTIONS]\n"
        "This program allows to specify, how many threads will be used for reading files "
        "(each thread gets its own files) and how many threads will be used for searching in each "
        "file, which is currently being read.\n"
        "\nOptions:"
        "\n\t-t1  NUM\t read NUM files in parallel"
        "\n\t\t\t min: %1%, max: %2%, single thread (default): %1%"
        "\n\t-t2  NUM\t run search in each file with max NUM threads"
        "\n\t\t\t min: %3%, max: %4%, single thread (default): %3%"
        "\n\t-sb  NUM\t split file to segments of size NUM and search in segments in parallel"
        "\n\t\t\t min: %5%, max: %6% (default: %7%)"
        "\n\t\t\t use with -t2 >= 1"
        "\n\t-rb  NUM\t size of file reading buffer. Best option is to set it to L3 cache size."
        "\n\t\t\t each call of fread() reads up to NUM characters"
        "\n\t\t\t min: %8%, max: %9% (default: %10%)"
        "\n\t-v      \t verbose"} 
        % THR_MIN % THR_MAX % THR_MIN % THR_MAX % SBUF_MIN % SBUF_MAX 
        % SBUF_DEF % RBUF_MIN % RBUF_MAX % RBUF_DEF);
    const string manual_single = boost::str(boost::format{"Usage: search PATH PATTERN [OPTIONS]\nOptions:"
        "\n\t-rb  NUM\t size of file reading buffer. Best option is to set it to L3 cache size."
        "\n\t\t\t min: %1%, max: %2% (default: %3%)"
        "\n\t-v      \t verbose"}  
        % RBUF_MIN % RBUF_MAX % RBUF_DEF);
    const string verb_path = "Searching in path (file): ";
    const string verb_needle = "Seraching for string: ";
    const string verb_t1 = "Using -t1 threads: ";
    const string verb_t2 = "Using -t2 threads: ";
    const string verb_rbuf = "Using read buffer of size: ";
    const string verb_sbuf = "Using segment buffer of size: ";


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
        % SBUF_MIN % SBUF_MAX);
    const string wrong_value_ibuf = boost::str(boost::format{"Error: use min %1% and max %2% for reading buffer size."} 
        % RBUF_MIN % RBUF_MAX);
    const string match_vs_segment = "Error: provided segment length is smaller than length of string, which is to be "
        "found in this segment.";
    const string read_buf_vs_segment = "Error: read buffer can't be smaller than segment size.";
    const string rbuf_gt_needle = "Error: reading buffer is shorter than string to search.";
    const string expect_num = "Expected number after switch: ";

    // program errors:
    const string file_error = "Unexpected error: ";
    const string file_size_error = "Unable to determine file size: ";
    const string file_open_error = "Can't open file: ";
    const string segment_vs_buffer = "Reading buffer size must be at bigger than segment size "
        " + length of the string to be matched + length of prefix and sufix to be printed.";
    const string warn_thread_vs_segment = "Read buffer size offers not enought segments for threads. "
        "Try to change ratio of reading buffer size, segment size and number of threads. Program will "
        "not stop, but it may be ineffective.";
    const string read_less = "Error: read function returned less characters as expected. On file: ";
    const string read_less_again = "Error: read function once again returned less characters as expected. "
        " Due to this error, skipping problem file: ";
    const string search_exception = "Error: an exception occured during searching in string: ";
    const string main_exception = "Internal exception: ";
    const string impossible = "Fatal error. Situation, which is not posibble, just happend.";
}

#endif