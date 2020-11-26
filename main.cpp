/*
    Program for parallel string search in files, folders and
    sub folders.  
*/

#include <iostream>
#include <filesystem>
#include <string>
#include "const.h"
#include "util/task_parallelizer.h"
#include "file_iterate.h"

using namespace std;
namespace fs = filesystem;

// wrapper function for std::stoi()
// handles stoi() exception, returns 1 if no exception, 0 otherwise
int stoi_exception(int *t_out, const char* t_in)
{
    try {
        *t_out = stoi(t_in);
        return 1;
    } catch (exception& e) {
        return 0;
    }
}

// print manual and exit program:
inline int arg_error_exit()
{
    cout << progconst::manual << endl;
    return EXIT_FAILURE;
}

// program main:
// parse and validate program arguments
// run file search
int main(int argc, char **argv)
{
    // chceck and process cmd arguments:
    if (argc < 3) {
        // not enough arguments:
        cout << progconst::arg_few << endl;
        return arg_error_exit();
    }
    // path and string to be found:
    string path = string(argv[1]);
    string match = argv[2];
    const unsigned match_len = match.length();
    
    // parse switches:
    int arg_t1 = (int) progconst::THR_MIN;
    int arg_t2 = (int) progconst::THR_MIN;
    int arg_rbuf = (int) progconst::BUF_NDEF;
    int arg_sbuf = (int) progconst::BUF_NDEF;
    int arg_idx = 3;
    while (arg_idx + 1 < argc) {
        // check switch and value:
        if (string(argv[arg_idx]) ==  progconst::switch_t1){
            // switch "-t1"
            if (!stoi_exception(&arg_t1, argv[arg_idx + 1])) {
                cout << progconst::not_number << argv[arg_idx + 1] << endl;
                return arg_error_exit();
            }
        } else if (string(argv[arg_idx]) ==  progconst::switch_t2){
            // switch "-t2"
            if (!stoi_exception(&arg_t2, argv[arg_idx + 1])) {
                cout << progconst::not_number << argv[arg_idx + 1] << endl;
                return arg_error_exit();
            }
        } else if (string(argv[arg_idx]) ==  progconst::switch_sbuf){
            // switch "-buf"
            if (!stoi_exception(&arg_sbuf, argv[arg_idx + 1])) {
                cout << progconst::not_number << argv[arg_idx + 1] << endl;
                return arg_error_exit();
            }
        } else if (string(argv[arg_idx]) ==  progconst::switch_rbuf){
            // switch "-ib"
            if (!stoi_exception(&arg_rbuf, argv[arg_idx + 1])) {
                cout << progconst::not_number << argv[arg_idx + 1] << endl;
                return arg_error_exit();
            }
        } else {
            // unknown switch
            cout << progconst::not_switch << argv[arg_idx] << endl;
            return arg_error_exit();
        }

        // set to next switch parse:
        arg_idx += 2;
    }

    // check values:
    if (arg_t1 < (int) progconst::THR_MIN 
        || arg_t1 > (int) progconst::THR_MAX) {
        cout << progconst::wrong_value_t1 << endl;
        return arg_error_exit();
    }
   if (arg_t2 < (int) progconst::THR_MIN 
        || arg_t2 > (int) progconst::THR_MAX) {
        cout << progconst::wrong_value_t2 << endl;
        return arg_error_exit();
    }

    // if segment was not provided use default
    // if segmetn was provided, check if in range:
    if (arg_sbuf == progconst::BUF_NDEF) {
        if (match_len > progconst::SBUF_DEF) {
            arg_sbuf = match_len * progconst::SBUF_DEF_RATIO;
        } else {
            arg_sbuf = progconst::SBUF_DEF;
        }
    } else if (arg_sbuf < (int) progconst::SBUF_MIN
        || arg_sbuf > (int) progconst::SBUF_MAX) {
        cout << progconst::wrong_value_buf << endl;
        return arg_error_exit();
    }

    // if read buffer size was not provided, use default
    // otherwise check if in range:
    if (arg_rbuf == (int) progconst::BUF_NDEF) {
        if (arg_sbuf > (int) progconst::RBUF_DEF) {
            arg_rbuf = arg_sbuf * progconst::RBUF_DEF_RATIO;
        } else {
            arg_rbuf = progconst::RBUF_DEF;
        }
    } else if (arg_rbuf < (int) progconst::RBUF_MIN 
        || arg_sbuf > (int) progconst::RBUF_MAX) {
        cout << progconst::wrong_value_ibuf << endl;
        return arg_error_exit();
    }

    // check file path:
    if (!fs::exists(string(argv[1]))) {
        cout << progconst::wrong_path << argv[1] << endl;
        return EXIT_FAILURE;
    }

    // check, if segment size and read buffer siez is 
    // bigger than string of string to be matched:
    if (arg_sbuf < (int) match_len) {
        cout << progconst::match_vs_segment << endl;
        return EXIT_FAILURE;
    }

    if (arg_rbuf < arg_sbuf) {
        cout << progconst::read_buf_vs_segment << endl;
        return EXIT_FAILURE;
    }

    // struct with program parameters:
    const struct job_details data[3] = {
        {nullptr, (unsigned) arg_t1, progconst::BUF_NDEF},
        {(int*) &arg_rbuf, (unsigned) arg_t2, (unsigned) arg_sbuf},
        {(string*) &match, progconst::THR_MIN, progconst::BUF_NDEF}
    };

    // run search:
    try {
        FileIterate search(data, 3);
        search.start(path);   
    } catch(const fs::filesystem_error& e) {
        std::cerr << progconst::file_error << e.what() << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}