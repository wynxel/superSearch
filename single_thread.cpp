/*
    This is test version of main program. 
    It is only single thread.
    It is created from main (superSearch) code
    after removing thread and synchronization statements.  
*/

#include <iostream>
#include <filesystem>
#include <string>
#include "const.h"

using namespace std;
namespace fs = filesystem;

// global variables:
int rbuf_len = (int) progconst::BUF_NDEF;
char* rbuf;
string needle;
string path;
unsigned needle_len;
const unsigned pref_suf_max_char = 7;
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

// free memory:
void free_mem(){
    delete[] rbuf;
}

// print manual and exit program:
inline int print_man_and_return_end()
{
    cout << progconst::manual_single << endl;
    return false;
}

// help function for escaping \t and \n characters
// and for copying prefix-sufix
bool create_escaped_substring
    (char* buf, unsigned buf_len, const char* from, const unsigned length)
    {
        unsigned counter = 0;
        for (unsigned i = 0; i < length; i++) {
            if (from[i] == 9 || from[i] == 10) {
                counter += 2;
            } else {
                counter++;
            }
        }
        if (buf_len < counter) {
            return false;
        }
        unsigned res_i = 0;
        for (unsigned i = 0; i < length; i++, res_i++) {
            if (from[i] == 9) {
                buf[res_i] = '\\';
                buf[++res_i] = 't';
            } else if (from[i] == 10) {
                buf[res_i] = '\\';
                buf[++res_i] = 'n';
            } else {
                buf[res_i] = from[i];
            }
        }
        buf[counter] = 0;
        return true;
    }

void print_result(string filename, const char* prefix, 
    const char* sufix, const unsigned offset)
{
        cout << "<" << filename << ">(<" << offset << ">):";
        cout << "<" << prefix << ">...<" << sufix << ">\n";
}

void search(const unsigned offset, const bool last, string filename)
{
    // check, if we are at start of file, in the file, or in the end:
    // (there are little differences)
    const unsigned start_padd = 
        offset == 0 ? 0 : progconst::PREFIX_LEN;
    const unsigned end_padd = 
        last ? 0 : progconst::SUFIX_LEN;

    // check if enough length:
    if (needle.length() + start_padd + end_padd > (unsigned) rbuf_len) {
        return;
    }
    // search for string:
    char* start = rbuf;
    char* end = start + rbuf_len;
    char* search_from = start + start_padd;
    char* search_to = start + rbuf_len - end_padd;
    char* position;
    while (search_from < search_to) {
        try {
            position = search(search_from, search_to,
                boyer_moore_searcher(needle.begin(), needle.end()));
        } catch (exception& e) {
            cerr << progconst::search_exception << e.what() << endl;
            return;
        }
        if (position != search_to) {
            // allocate prefix-sufix:
            unsigned pref_len = 
                progconst::PREFIX_LEN < (position - start) ? 
                progconst::PREFIX_LEN : (position - start);
            unsigned sufx_len = 
                progconst::SUFIX_LEN < (end - position - needle_len) ? 
                progconst::SUFIX_LEN : (end - position - needle_len);
            // create strings:
            char prefix[pref_suf_max_char];
            if (!create_escaped_substring
                (prefix, pref_suf_max_char, position - pref_len, pref_len)) {
                    cerr << progconst::impossible << endl;
                }
            char sufix[pref_suf_max_char];
            if(!create_escaped_substring
                (sufix, pref_suf_max_char, position + needle_len, sufx_len)) {
                    cerr << progconst::impossible << endl;
                }
            // print:
            print_result(filename, prefix, sufix, 
                ((unsigned)(position - start)) + offset);
        } else {
            break;
        }
        search_from = position + 1;
    }
}

void search_in_file(const fs::path t_path)
{    
    // get file size: 
    uintmax_t file_size;
    try{
        file_size = fs::file_size(t_path);
    } catch (exception &e) {
        cerr << progconst::file_size_error << e.what() << endl;
        return;
    }

    // open file:
    FILE* file = fopen(t_path.string().c_str(), "r");
    if (file == nullptr) {
        cerr << progconst::file_open_error << t_path.string() << endl;
        return;
    }

    // read in cycel:
    uintmax_t size_left = file_size;
    bool read_error_again = false;
    while(size_left > 0) {
        // read to buffer
        unsigned to_read = 
            (uintmax_t) rbuf_len < size_left ? 
            rbuf_len : size_left;
        const size_t read = fread(rbuf, sizeof rbuf[0], to_read, file);
        // check length:
        if (to_read != read) {
            // we will accept one error...
            if (read_error_again) {
                cerr << progconst::read_less_again << t_path.string() << endl;
                break;
            }
            cerr << progconst::read_less << t_path.string() << endl;
            read_error_again = true;
        }
        // search in buffer:
        // (t_path.string() or t_path.filename().string())
        search(file_size - size_left, 
            (size_left == read) ? true : false, 
            t_path.string());
        size_left -= read;
    }
    fclose(file);
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
        if (string(argv[arg_idx]) ==  progconst::switch_rbuf){
            // switch "-rb"
            if (!save_int_swich_value
                (rbuf_len, progconst::switch_rbuf, argv, argc, arg_idx + 1)) {
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

// check read buffer length
// if not provided, use defaults
// then check read buffer vs needle length:
inline bool check_read_buffer_len()
{
    // if read buffer size was not provided, use default
    // otherwise check if in range:
    if (rbuf_len == (int) progconst::BUF_NDEF) {
            rbuf_len = progconst::RBUF_DEF;
    } else if (rbuf_len < (int) progconst::RBUF_MIN 
        || rbuf_len > (int) progconst::RBUF_MAX) {
        cout << progconst::wrong_value_ibuf << endl;
        return print_man_and_return_end();
    }

    // check, if read buffer siez is 
    // bigger than string to be matched:
    if ((unsigned) rbuf_len + progconst::SUFIX_LEN + progconst::PREFIX_LEN 
        < needle_len) {
        cout << progconst::rbuf_gt_needle << endl;
        return EXIT_FAILURE;
    }
    return true;
}

void print_details()
{
    cout << progconst::verb_path << "\"" << path << "\"" << endl;
    cout << progconst::verb_needle << "\"" << needle << "\"" << endl;
    cout << progconst::verb_rbuf << rbuf_len << endl;
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

    // check reading and segment buffer sizes:
    if(!check_read_buffer_len()) {
        return EXIT_FAILURE;
    }

    // check file path:
    if (!fs::exists(string(argv[1]))) {
        cout << progconst::wrong_path << argv[1] << endl;
        return EXIT_FAILURE;
    }

    // create buffer for reading:
    rbuf = new char[rbuf_len];

    // be verbose?
    if (arg_verb) {
        print_details();
    }

    // run search:
    try {
        // check, if path argument is file or directory: 
        if (fs::is_regular_file(path)) {
            search_in_file(path);
        } else {
            for (auto& fs_item: fs::recursive_directory_iterator(path)) {
                if (fs::is_regular_file(fs_item.path())) {
                    search_in_file(fs_item.path().string());
                }
            }
        }  
    } catch (fs::filesystem_error& e) {
        std::cerr << progconst::file_error << e.what() << endl;
        free_mem();
        return EXIT_FAILURE;
    } catch (exception& e) {
        std::cerr << progconst::main_exception << e.what() << endl;
        free_mem();
        return EXIT_FAILURE;
    }
    free_mem();
    return EXIT_SUCCESS;
}