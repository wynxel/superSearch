/*
    see file_iterate.h
*/
#include "file_iterate.h"

// cosntructor
FileIterate::FileIterate(const struct job_details t_jobs[], 
            const unsigned t_job_num, 
            TaskContainer* t_super_job_class, const int t_id) 
            : TaskParallelizer(t_jobs, t_job_num, 
                t_super_job_class, t_id){};

// destructor
FileIterate::~FileIterate(){};

// main function:
void FileIterate::start(string &t_path)
{
    // check, if path argument is file or directory: 
    if (fs::is_regular_file(t_path)) {
        fs::path sub_path = fs::path(t_path);
        call_sub_job(sub_path);
    } else {
        for (auto& fs_item: fs::recursive_directory_iterator(t_path)) {
            if (fs::is_regular_file(fs_item.path())) {
                fs::path sub_path = fs_item.path();
                call_sub_job(sub_path);
            }
        }
    }  
}

void FileIterate::start()
{
    TaskParallelizer<string, string, string, FileRead>* super_class = 
        (TaskParallelizer<string, string, string, FileRead>*) get_super_class();
    string path = super_class->next_job_argument();
    start(path);
}

// sub_results are not used
void FileIterate::process_sub_results()
{
    return;
}