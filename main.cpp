/*

                TODO

*/

#include <iostream>
#include <filesystem>
#include <string>
#include "const.h"
#include "taskParallelizer.h"
#include "data.h"

using namespace std;
namespace fs = filesystem;

// wrapper function for std::stoi()
// handles stoi() exception, returns 1 if no exception, 0 otherwise
int stoi_exception(int *t_out, const char* t_in)
{
    try {
        *t_out = stoi(t_in);
        return 1;
    } catch (exception e) {
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
    
    // parse switches:
    int arg_t1 = progconst::THR_MIN;
    int arg_t2 = progconst::THR_MIN;
    int arg_buf = progconst::BUF_DEF;
    int arg_idx = 3;
    while (arg_idx + 1 < argc) {
        // check switch and value:
        if (string(argv[arg_idx]) ==  progconst::arg_t1){
            // switch "-t1"
            if (!stoi_exception(&arg_t1, argv[arg_idx + 1])) {
                cout << progconst::not_number << argv[arg_idx + 1] << endl;
                return arg_error_exit();
            }
        } else if (string(argv[arg_idx]) ==  progconst::arg_t2){
            // switch "-t2"
            if (!stoi_exception(&arg_t2, argv[arg_idx + 1])) {
                cout << progconst::not_number << argv[arg_idx + 1] << endl;
                return arg_error_exit();
            }
        } else if (string(argv[arg_idx]) ==  progconst::arg_t2_buf){
            // switch "-buf"
            if (!stoi_exception(&arg_buf, argv[arg_idx + 1])) {
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
    if (arg_t1 < progconst::THR_MIN || arg_t1 > progconst::THR_MAX) {
        cout << progconst::wrong_value_t1 << endl;
        return arg_error_exit();
    }
   if (arg_t2 < progconst::THR_MIN || arg_t2 > progconst::THR_MAX) {
        cout << progconst::wrong_value_t2 << endl;
        return arg_error_exit();
    }
   if (arg_buf < progconst::BUF_MIN || arg_buf > progconst::BUF_MAX) {
        cout << progconst::wrong_value_buf << endl;
        return arg_error_exit();
    }

    // check file path:
    if (!fs::exists(string(argv[1]))) {
        cout << progconst::wrong_path << argv[1] << endl;
        return EXIT_FAILURE;
    }

    // store arguments: directory, string to match, multi thread settings:
    const struct prog_data data = 
        {string(argv[2]), string(argv[1]), arg_t1, arg_t2, arg_buf};

    // create class for managing program output:
    // TODO

    // run search:
    try {
        //FilesystemIterate search(data);
        int i = 0;
    } catch(const fs::filesystem_error& e) {
        std::cerr << progconst::file_error << e.what() << endl;
        return EXIT_FAILURE;
    }
    cout << data.path.string() << ", " << data.to_find << ", " << data.t1 << ", " << data.t2 << ", " << data.b2_buf << endl;
    return EXIT_SUCCESS;
}