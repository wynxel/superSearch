/*
    see string_search.h
*/
#include "string_search.h"

// constructor:
StringSearch::StringSearch(const struct job_details t_jobs[], 
            const unsigned t_job_num, 
            TaskContainer* t_super_job_class, const int t_id) 
            : TaskParallelizer(t_jobs, t_job_num, t_super_job_class, t_id){};

// destructor:
StringSearch::~StringSearch(){};

// help function for escaping \t and \n characters
// and for copying prefix-sufix
char* StringSearch::create_escaped_substring
    (const char* from, const unsigned length)
    {
        unsigned counter = 0;
        for (unsigned i = 0; i < length; i++) {
            if (from[i] == 9 || from[i] == 10) {
                counter += 2;
            } else {
                counter++;
            }
        }
        char* res_str = new char[counter + 1];
        unsigned res_i = 0;
        for (unsigned i = 0; i < length; i++, res_i++) {
            if (from[i] == 9) {
                res_str[res_i] = '\\';
                res_str[++res_i] = 't';
            } else if (from[i] == 10) {
                res_str[res_i] = '\\';
                res_str[++res_i] = 'n';
            } else {
                res_str[res_i] = from[i];
            }
        }
        res_str[counter] = 0;
        return res_str;
    }

// main function:
void StringSearch::start(segment* &t_seg)
{
    string* needle = (string*) m_job_details->job_detail;
    unsigned needle_len = needle->length();

    // check, if we are at start of file, in the file, or in the end:
    // (there are little differences)
    const unsigned start_padd = 
        t_seg->offset == 0 ? 0 : progconst::PREFIX_LEN;
    const unsigned end_padd = 
        t_seg->last ? 0 : progconst::SUFIX_LEN;

    // check if enough length:
    if (needle->length() + start_padd + end_padd > t_seg->length) {
        return;
    }
    // search for string:
    char* start = t_seg->data;
    char* end = start + t_seg->length;
    char* search_from = start + start_padd;
    char* search_to = start + t_seg->length - end_padd;
    char* position;
    while (search_from < search_to) {
        try {
            position = search(search_from, search_to,
                boyer_moore_searcher(needle->begin(), needle->end()));
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
            char* prefix = create_escaped_substring(position - pref_len, pref_len);
            char* sufix = create_escaped_substring(position + needle_len, sufx_len);
            // create and push result object:
            result* res = new result{prefix, sufix, ((unsigned)(position - start)) + t_seg->offset};
            ((TaskParallelizer<fs::path, segment*, result*, StringSearch>*)get_super_class())->put_sub_result(res);
        } else {
            break;
        }
        search_from = position + 1;
    }
}

void StringSearch::start()
{
    TaskParallelizer<fs::path, segment*, result*, StringSearch>* super_class = 
        (TaskParallelizer<fs::path, segment*, result*, StringSearch>*) get_super_class();
    segment* next_segm = super_class->next_job_argument();
    start(next_segm);
}

// this class does not have any sub-jobs,
// so it does not process any sub-job results
void StringSearch::process_sub_results()
{
    return;
}