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

// program arguments:
// (global variables)
string path;
string needle;
unsigned needle_len;
int arg_t1 = (int) progconst::THR_MIN;
int arg_t2 = (int) progconst::THR_MIN;
int arg_rbuf = (int) progconst::BUF_NDEF;
int arg_sbuf = (int) progconst::BUF_NDEF;
bool arg_verb = progconst::VERB_DEF;


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
inline bool print_man_and_return_end()
{
    cout << progconst::manual << endl;
    return false;
}

// function used in argument parse cycle
// function is used for checking
// and storing int value
// t_arg - variable to which will be int value stored
// t_switch - swich associated with this int value
//  (this switch string is used for error print)
// t_argv, t_argc - c++ main() parameters
// t_index - index, on which desired int value should be placed
bool save_int_swich_value(int &t_arg, const string t_switch, 
    const char** t_argv, const int t_argc, const int t_index)
{
    // check, if we are in argc range:
    if (t_index >= t_argc) {
        cerr << progconst::expect_num << t_switch << endl;
        return print_man_and_return_end();
    }
    if (!stoi_exception(&t_arg, t_argv[t_index])) {
        cout << progconst::not_number << t_argv[t_index] << endl;
        return print_man_and_return_end();
    }
    return true;
}

// function for parsing cmd arguments
// switch values are stored to global variables
inline bool parse_cmd_arg(const int argc, const char **argv)
{
    int arg_idx = 3;
    while (arg_idx < argc) {
        // check switch and value:
        if (string(argv[arg_idx]) ==  progconst::switch_t1){
            // switch "-t1"
            if (!save_int_swich_value
                (arg_t1, progconst::switch_t1, argv, argc, arg_idx + 1)) {
                return false;
            }
            arg_idx += 2;
        } else if (string(argv[arg_idx]) ==  progconst::switch_t2){
            // switch "-t2"
            if (!save_int_swich_value
                (arg_t2, progconst::switch_t2, argv, argc, arg_idx + 1)) {
                return false;
            }
            arg_idx += 2;
        } else if (string(argv[arg_idx]) ==  progconst::switch_sbuf){
            // switch "-sb"
            if (!save_int_swich_value
                (arg_sbuf, progconst::switch_sbuf, argv, argc, arg_idx + 1)) {
                return false;
            }
            arg_idx += 2;
        } else if (string(argv[arg_idx]) ==  progconst::switch_rbuf){
            // switch "-rb"
            if (!save_int_swich_value
                (arg_rbuf, progconst::switch_rbuf, argv, argc, arg_idx + 1)) {
                return false;
            }
            arg_idx += 2;
        } else if (string(argv[arg_idx]) ==  progconst::switch_verb){
            // switch "-v"
            arg_verb = true;
            arg_idx += 1;
        } else if (string(argv[arg_idx]) ==  progconst::switch_h1){
            // switch "-h"
            return print_man_and_return_end();
        } else {
            // unknown switch
            cout << progconst::not_switch << argv[arg_idx] << endl;
            return print_man_and_return_end();
        }
    }
    return true;
}

// check, if thread numbers are in allowed range:
inline bool check_num_of_threads()
{
    if (arg_t1 < (int) progconst::THR_MIN 
        || arg_t1 > (int) progconst::THR_MAX) {
        cout << progconst::wrong_value_t1 << endl;
        return print_man_and_return_end();
    }
    if (arg_t2 < (int) progconst::THR_MIN 
        || arg_t2 > (int) progconst::THR_MAX) {
        cout << progconst::wrong_value_t2 << endl;
        return print_man_and_return_end();
    }
    return true;
}

// check segment buffer length and read buffer length
// if not provided, use defaults
// then check segment buffer vs read buffer vs needle 
// length:
inline bool check_read_and_segment_buffer_len(){
    // if segment was not provided use default/based on 
    // needle length
    // if segmetn was provided, check if in range:
    if (arg_sbuf == progconst::BUF_NDEF) {
        if (needle_len > progconst::SBUF_DEF) {
            arg_sbuf = needle_len * progconst::SBUF_DEF_RATIO;
        } else {
            arg_sbuf = progconst::SBUF_DEF;
        }
    } else if (arg_sbuf < (int) progconst::SBUF_MIN
        || arg_sbuf > (int) progconst::SBUF_MAX) {
        cout << progconst::wrong_value_buf << endl;
        return print_man_and_return_end();
    }

    // if read buffer size was not provided, use default/based
    // on segment length
    // otherwise check if in range:
    if (arg_rbuf == (int) progconst::BUF_NDEF) {
        if (arg_sbuf > (int) progconst::RBUF_DEF) {
            arg_rbuf = arg_sbuf * progconst::RBUF_DEF_RATIO;
        } else {
            arg_rbuf = progconst::RBUF_DEF;
        }
    } else if (arg_rbuf < (int) progconst::RBUF_MIN 
        || arg_rbuf > (int) progconst::RBUF_MAX) {
        cout << progconst::wrong_value_ibuf << endl;
        return print_man_and_return_end();
    }

    // check, if segment size and read buffer siez is 
    // bigger than string to be matched (needle):
    if (arg_sbuf < (int) needle_len) {
        cout << progconst::match_vs_segment << endl;
        return false;
    }

    // check, if reading buffer has enought length
    // for segment buffer:
    if (arg_rbuf < arg_sbuf) {
        cout << progconst::read_buf_vs_segment << endl;
        return false;
    }
    return true;
}

void print_details()
{
    cout << progconst::verb_path << "\"" << path << "\"" << endl;
    cout << progconst::verb_needle << "\"" << needle << "\"" << endl;
    cout << progconst::verb_t1 << arg_t1 << endl;
    cout << progconst::verb_t2 << arg_t2 << endl;
    cout << progconst::verb_rbuf << arg_rbuf << endl;
    cout << progconst::verb_sbuf << arg_sbuf << endl;
    cout << endl;
}

// program main:
// parse and validate program arguments
// run file search
int main(const int argc, const char **argv)
{
    // chceck and process cmd arguments:
    if (argc < 3) {
        // not enough arguments:
        cout << progconst::arg_few << endl;
        return print_man_and_return_end();
    }
    // path and string to be found:
    path = string(argv[1]);
    needle = argv[2];
    needle_len = needle.length();
    
    // parse switches:
    if(!parse_cmd_arg(argc, argv)) {
        return EXIT_FAILURE;
    }

    // check thread switches:
    if(!check_num_of_threads()) {
        return EXIT_FAILURE;
    }

    // check reading and segment buffer sizes:
    if(!check_read_and_segment_buffer_len()) {
        return EXIT_FAILURE;
    }

    // check file path:
    if (!fs::exists(string(argv[1]))) {
        cout << progconst::wrong_path << argv[1] << endl;
        return EXIT_FAILURE;
    }

    // create struct with final program parameters:
    const struct job_details prog_settings[3] = {
        {nullptr, (unsigned) arg_t1, progconst::BUF_NDEF},
        {(int*) &arg_rbuf, (unsigned) arg_t2, (unsigned) arg_sbuf},
        {(string*) &needle, progconst::THR_MIN, progconst::BUF_NDEF}
    };

    // be verbose?
    if (arg_verb) {
        print_details();
    }

    // run search:
    try {
        FileIterate search(prog_settings, 3);
        search.start(path);   
    } catch (fs::filesystem_error& e) {
        std::cerr << progconst::file_error << e.what() << endl;
        return EXIT_FAILURE;
    } catch (exception& e) {
        std::cerr << progconst::main_exception << e.what() << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}