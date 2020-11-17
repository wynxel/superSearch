/*

                TODO
                
    

*/
#include "file_read.h"

FileRead::FileRead(const struct job_details t_jobs[], 
            const unsigned t_job_num, 
            TaskContainer* t_super_job_class) 
            : TaskParallelizer(t_jobs, t_job_num, t_super_job_class){cout << "FR1\n";};
FileRead::~FileRead(){cout << "FR1\n";};

void FileRead::start(const fs::path &t_path)
{
    // just test:
    cout << t_path << "\n";
}

void FileRead::start()
{
    if (m_super_job_class == nullptr) {
        start(next_job_argument());
    } else {
        start(((TaskParallelizer<string, fs::path, Terminator>*)m_super_job_class)->next_job_argument());
    }
}