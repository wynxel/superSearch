/*

                TODO
                
    

*/
#include "file_iterate.h"
#include "task_parallelizer.h"
#include "file_read.h"

FileIterate::FileIterate(const struct job_details t_jobs[], 
            const unsigned t_job_num, 
            TaskContainer* t_super_job_class) 
            : TaskParallelizer(t_jobs, t_job_num, t_super_job_class){};

FileIterate::~FileIterate(){};

void FileIterate::start(const string &t_path)
{
    // check, if path argument is file or directory: 
    if (fs::is_regular_file(t_path)) {
        call_sub_job(fs::path(t_path));
    } else {
        for (auto& fs_item: fs::recursive_directory_iterator(t_path)) {
            if (fs::is_regular_file(fs_item.path())) {
                call_sub_job(fs_item.path());
            }
        }
    }  
}

void FileIterate::start()
{
    TaskParallelizer<string, string, string, FileRead>* super_class = 
        (TaskParallelizer<string, string, string, FileRead>*) get_super_class();
    if (super_class == nullptr) {
        start(next_job_argument());
    } else {
        start(super_class->next_job_argument());
    }
}