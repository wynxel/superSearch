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
    const unsigned SBUF_MIN = 128;
    const unsigned SBUF_MAX = 1500000;
    const unsigned SBUF_DEF = 8192;
    const unsigned RBUF_MIN = 512;
    const unsigned RBUF_DEF = 6000000;
    const unsigned RBUF_MAX = 500000000;
    const unsigned SBUF_DEF_RATIO = 2;
    const unsigned RBUF_DEF_RATIO = 10;

    // switches: 
    const string switch_t1 = "-t1";
    const string switch_t2 = "-t2";
    const string switch_rbuf = "-ib";
    const string switch_sbuf = "-buf";

    // manual: 
    const string manual = boost::str(boost::format{"Ussage: search PATH PATTERN [OPTIONS]\nOptions:"
        "\n\t-t1  NUM\t process NUM files in parallel"
        "\n\t\t\t min: %1%, max: %2%, single thread (default): %1%"
        "\n\t-t2  NUM\t run search in each file with max NUM threads"
        "\n\t\t\t min: %3%, max: %4%, single thread (default): %3%"
        "\n\t-buf NUM\t if -t2 > 1, split file to segments of size NUM and search in segments in parallel"
        "\n\t\t\t min: %5%, max: %6% (default: %7%)"
        "\n\t-ib  NUM\t size of file reading buffer. Best option is to set it to L3 cache size."
        "\n\t\t\t min: %8%, max: %9% (default: %10%)"} 
        % THR_MIN % THR_MAX % THR_MIN % THR_MAX % SBUF_MIN % SBUF_MAX 
        % SBUF_DEF % RBUF_MIN % RBUF_MAX % RBUF_DEF);

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

    // program errors:
    const string file_error = "Unexpected error: ";
    const string file_size_error = "Unable to determine file size: ";
    const string file_open_error = "Can't open file: ";
    const string segment_vs_buffer = "Segment size is bigger than read buffer size.";
    const string warn_thread_vs_segment = "Read buffer size offers not enought segments for threads. "
        "Try to change ratio of reading buffer size, segment size and number of threads. Program will "
        "not stop, but it may be ineffective.";
    const string read_less = "Error: read function returned less characters as expected. On file: ";
    const string read_less_again = "Error: read function once again returned less characters as expected. "
        " Due to this error, skipping problem file: ";


    // runtime exceptions:
}

#endif