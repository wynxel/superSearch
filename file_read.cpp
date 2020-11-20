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
    // TODO, pozor, m_next_job (ci jak sa vola ten vektor) musi byt 
    // vektor tuplov: <offset, data>
    // a potom sub job-trieda musi vracat <offest, string> - string lebo sa ma nieco ine vypisovat ak nie je cely...
    cout << t_path << "\n";
}

void FileRead::start()
{
    TaskParallelizer<string, fs::path, string, FileRead>* super_class = 
        (TaskParallelizer<string, fs::path, string, FileRead>*) get_super_class();
    start(super_class->next_job_argument());
}

void FileRead::process_sub_results()
{
    return;
}