/*

                TODO
                
    

*/
#include "file_iterate.h"
#include "file_read.h"

inline void FileIterate::assign_sub_job_class(const unsigned int t_thread_num)
{
    for (int i = 0; i < t_thread_num; i++) {
        int f = 15;
        // TODO:
        m_sub_job_class.push_back(new FileRead());
    }
} 

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
    if (m_super_job_class == nullptr) {
        start(next_job_argument());
    } else {
        start(((TaskParallelizer<string, string>*)m_super_job_class)->next_job_argument());
    }
}