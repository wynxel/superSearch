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

    // switches: 
    const string arg_t1 = "-t1";
    const string arg_t2 = "-t2";
    const string arg_t2_buf = "-buf";

    // manual: 
    const string manual = boost::str(boost::format{"Ussage: search PATH PATTERN [OPTIONS]\nOptions:"
        "\n\t-t1  NUM\t process NUM files in parallel"
        "\n\t\t\t min: %1%, max: %2%, single thread (default): %1%"
        "\n\t-t2  NUM\t run search in each file with max NUM threads"
        "\n\t\t\t min: %3%, max: %4%, single thread (default): %3%"
        "\n\t-buf NUM\t if -t2 > 1, split file to segments of size NUM and search in segments in parallel"
        "\n\t\t\t min: %5%, max: %6% (X %7% bites, default: %8%)"} 
        % THR_MIN % THR_MAX % THR_MIN % THR_MAX % BUF_MIN % BUF_MAX % BUF_MULT % BUF_DEF);

    // argument errors:
    const string arg_few = "Not enough arguments.";
    const string not_number = "Expected number, got: ";
    const string not_switch = "Unknown switch: ";
    const string wrong_path = "Provided path does not exists: ";
    const string wrong_value_t1 = boost::str(boost::format{"Use min %1% and max %2% threads for -t1."} % THR_MIN % THR_MAX);
    const string wrong_value_t2 = boost::str(boost::format{"Use min %1% and max %2% threads for -t2."} % THR_MIN % THR_MAX);
    const string wrong_value_buf = boost::str(boost::format{"Use min %1% and max %2% for buffer size."} % BUF_MIN % BUF_MAX);

    // program errors:
    const string file_error = "Unexpected error: ";

    // runtime exceptions:
}

#endif