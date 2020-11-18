/*

                TODO
                
    

*/
#include "file_read.h"

FileRead::FileRead(const struct job_details t_jobs[], 
            const unsigned t_job_num, 
            TaskContainer* t_super_job_class) 
            : TaskParallelizer(t_jobs, t_job_num, t_super_job_class){};
FileRead::~FileRead(){};

void FileRead::start(const fs::path &t_path)
{
    // just test:
    cout << t_path << "\n";
}

void FileRead::start()
{
    TaskParallelizer<string, fs::path, string, FileRead>* super_class = 
        (TaskParallelizer<string, fs::path, string, FileRead>*) get_super_class();
    if (super_class == nullptr) {
        start(next_job_argument());
    } else {
        start(super_class->next_job_argument());
    }
}