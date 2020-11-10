/*
void FilesystemIterate::process_path()
{
    // check, if path argument is file or directory: 
    if (fs::is_regular_file(m_next_job.path)) {
        process_file(m_next_job.path);
    } else {
        for (auto& fs_item: fs::recursive_directory_iterator(m_next_job.path)) {
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


*/