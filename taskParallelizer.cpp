/*

                TODO
                
    

*/

#include "taskParallelizer.h"
#include "const.h"

// constructor:
template <typename S, typename T>
TaskParallelizer<S, T>::TaskParallelizer(const struct job_details t_job,
    const struct job_details t_last_job, 
    TaskParallelizer* t_super_job_class = nullptr) :
    m_my_job(t_job), 
    m_paralell((t_job[0].thread_number > progconst::THR_MIN) ? true : false),
    m_super_job_class(t_super_job_class)
    {
        // call function for setting m_sub_job_class:
        assign_sub_job_class();

        // start threads if in multithread mode:
        // TODO

        // start job:
        // TODO: toto tu nemoze byt, vsetky triedy by sa 
        // spustili po instancovani (aj tie sub, ktore este nemaju parametre!)
        // a je popletene (asi) vyznam stringu s prikazom v structe a to co je vo vektoroch!
        //do_job sa bude musiet spustat rucne? 
        // do_job();
    }

// pozor, smernik smernikov (je to pole tried)
template <typename S, typename T>
inline void TaskParallelizer<S, T>::assign_sub_job_class()
{
    m_sub_job_class = nullptr;
}

template <typename S, typename T>
const T TaskParallelizer<S, T>::next_job_argument()
{
    // TODO
}

template <typename S, typename T>
void TaskParallelizer<S, T>::call_sub_job(const T t_item)
{
    // check if multithread mode:
    if (m_paralell) {
        scoped_lock<mutex> locker(m_mutex);
        m_data.push_back(t_item);
    } else {
        // single thread, call directly:
        m_sub_job_class->do_job(t_item);
    }
}

template <typename S, typename T>
TaskParallelizer<S, T>::~TaskParallelizer()
{
    // pockat kym dobehnu
    // umazat alokovanu pamat
}

template <typename S, typename T>
void TaskParallelizer<S, T>::do_job(const S t_single_job = nullptr)
{
    // (odstranit virtual, resp presunut)
    // vybrat zo super zasobnika
    // zavolat na tom funkciu 
}

/*
void FilesystemIterate::process_path()
{
    // check, if path argument is file or directory: 
    if (fs::is_regular_file(m_data.path)) {
        process_file(m_data.path);
    } else {
        for (auto& fs_item: fs::recursive_directory_iterator(m_data.path)) {
            const filesystem::path path = fs_item.path();
            if (fs::is_regular_file(path)) {
                process_file(string(path));
            }
        }
    }  
}


// Function for processing single file.
// If in multithread mode, function only adds file path (string)
// to internal stack. This file will be later processed by some
// free thread.
// If in singlethread mode, function calls string search in file. 
void FilesystemIterate::process_file(const string &t_path)
{
    // check if multithread mode:
    if (m_t1_paralell) {
        scoped_lock<mutex> locker(m_mutex);
        m_filePaths.push_back(t_path);
        cout << "mtthrd: file add: " << t_path << "\n";
    } else {
        // TODO: single thread, direct search in file:
        cout << "sgthr: file add\n";
    }
}

// Function used in multithread file reading mode.
// Returns next file path (string) to be read 
// and processed. 
// If there are no files in file-path stack, 
// function returns empty string "".
// If file reading is not running in multi-thread
// mode, function returns empty string. 
const string FilesystemIterate::get_path()
{
    // check, if we are running multi-thread mode:
    if (m_t1_paralell) {
        const string ret_str = "";
        scoped_lock<mutex> locker(m_mutex);
        if (!m_filePaths.empty()) {
            ret_str = m_filePaths.back();
            m_filePaths.pop_back();
        }
        return ret_str;
    }
    return "";
}
*/