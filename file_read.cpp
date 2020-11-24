/*

                TODO
                
    

*/
#include "file_read.h"

FileRead::FileRead(const struct job_details t_jobs[], 
            const unsigned t_job_num, 
            TaskContainer* t_super_job_class) 
            : TaskParallelizer(t_jobs, t_job_num, t_super_job_class),
            m_buffer_len(*((int*) m_job_details->job_detail)),
            m_seg_len(m_job_details->job_segment_size)
            {
                // check values:
                if(m_buffer_len < progconst::IBUF_MIN
                    || m_buffer_len > progconst::IBUF_MAX) {
                        throw invalid_argument(progconst::wrong_value_ibuf);
                    }
                if(m_seg_len < progconst::BUF_MIN
                    || m_seg_len > progconst::BUF_MAX) {
                        throw invalid_argument(progconst::wrong_value_buf);
                    }

                unsigned desired_buf = *((int*) m_job_details->job_detail);
                unsigned desired_seg = m_job_details->job_segment_size;

                // check segment-buffer ratio:
                if (desired_seg > desired_buf) {
                    throw invalid_argument(progconst::segment_vs_buffer);
                }

                // alighn buffer-segment lengths:
                // [segment size][segment siez] ... [segment size -1] 
                // | -->               BUFFER SIZE                  |
                // Buffer size can be max: *((int*) m_job_details->job_detail)
                unsigned num_of_seg = desired_buf / desired_seg;
                unsigned reminder =  desired_buf % desired_seg;

                // check usefullness of multithread:
                if (m_parallel && num_of_seg < m_job_details->thread_number) {
                    cerr << progconst::warn_thread_vs_segment << endl;
                }

                if (reminder != (desired_seg - 1)) {
                    num_of_seg--;
                    m_buffer_len = num_of_seg * desired_seg + desired_seg - 1;
                } else {
                    m_buffer_len = desired_buf;
                }
                
                m_buffer = new char[m_buffer_len];
            };
            
FileRead::~FileRead()
{
    delete[] m_buffer;
    for (auto &elem : garbage) {
        delete elem;
    }
}

void FileRead::start(fs::path &t_path)
{
    // get file size: 
    uintmax_t file_size;
    try{
        file_size = fs::file_size(t_path);
    } catch (exception &e) {
        cerr << progconst::file_size_error << e.what() << endl;
        // !! TODO staci len return? Netreba este nieco?
        return;
    }

    // open file:
    uintmax_t general_file_offset = 0;
    FILE* file = fopen(t_path.string().c_str(), "r");
    if (file == nullptr) {
        cerr << progconst::file_open_error << t_path.string() << endl;
        // !! TODO staci len return? Netreba este nieco?
        return;
    }

    // read in cycel:
    uintmax_t size_left = file_size;
    while(size_left > 0) {
        // handle exceptions!

        // read to buffer
        unsigned to_read = m_buffer_len < size_left ? m_buffer_len : size_left;
        const size_t read = fread(m_buffer, sizeof m_buffer[0], to_read, file);
        // check length:
        if (to_read != read) {
            // CO TERAZ? TODO
            cout << "zle je\n";
        }
        // create segment, add to garbage collector, call sub job:
        unsigned index = 0;
        while (index < read){
            unsigned this_segment_length 
                = m_seg_len < (read - index) ? m_seg_len : (read - index);
            segment* seg = new segment
                {general_file_offset, this_segment_length, &m_buffer[index]};
            garbage.push_back(seg);
            call_sub_job(seg);
            index += this_segment_length;
        }
        size_left -= read;
        general_file_offset += read;
    }
    fclose(file);

   /*
    ...
    dalsie veci:
    volat garbage collector ked treba + kde sa bude cistit
    spracovavat navratovu hodnotu ked treba (ten "RESULT")
        vlastne nevieme ci ho bude pouzivat aj file_iterate
    */
}

void FileRead::start()
{
    TaskParallelizer<string, fs::path, string, FileRead>* super_class = 
        (TaskParallelizer<string, fs::path, string, FileRead>*) get_super_class();
    fs::path path = super_class->next_job_argument();
    start(path);
}

void FileRead::process_sub_results()
{
    return;
}