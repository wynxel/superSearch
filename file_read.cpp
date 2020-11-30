/*
    see file_read.h
*/
#include "file_read.h"

// constructor:
// save and check values
// allocate buffer
// for more details, see TaskParallelizer constructor/class description
FileRead::FileRead(const struct job_details t_jobs[], 
            const unsigned t_job_num, 
            TaskContainer* t_super_job_class, const int t_id) 
            : TaskParallelizer(t_jobs, t_job_num, t_super_job_class, t_id),
            m_sbuf_len(m_job_details->job_segment_size),
            m_overlap(((string*) t_jobs[1].job_detail)->length()
                 - 1 + progconst::PREFIX_LEN + progconst::SUFIX_LEN),
            m_rbuf_len(*((int*) m_job_details->job_detail))
            {
                // check m_rbuf_len, m_sbuf_len range:
                if(m_rbuf_len < progconst::RBUF_MIN
                    || m_rbuf_len > progconst::RBUF_MAX) {
                        throw invalid_argument(progconst::wrong_value_ibuf);
                    }
                if(m_sbuf_len < progconst::SBUF_MIN
                    || m_sbuf_len > progconst::SBUF_MAX) {
                        throw invalid_argument(progconst::wrong_value_buf);
                    }

                // check segment-buffer ratio:
                if (m_sbuf_len + m_overlap > m_rbuf_len) {
                    throw invalid_argument(progconst::segment_vs_buffer);
                }

                // check usefullness of multithread:
                if (m_parallel && 
                    (m_rbuf_len / m_sbuf_len) < m_job_details->thread_number) {
                    cerr << progconst::warn_thread_vs_segment << endl;
                }

                // crate buffer for reading:
                m_rbuf = new char[m_rbuf_len];
            };

// destructor:          
FileRead::~FileRead()
{
    delete[] m_rbuf;
    clear_collector();
}

// main function:
void FileRead::start(fs::path &t_path)
{
    // save filename for process_sub_results() function:
    m_file_path = t_path;
    
    // get file size: 
    uintmax_t file_size;
    try{
        file_size = fs::file_size(t_path);
    } catch (exception &e) {
        cerr << progconst::file_size_error << e.what() << endl;
        return;
    }

    // open file:
    FILE* file = fopen(t_path.string().c_str(), "r");
    if (file == nullptr) {
        cerr << progconst::file_open_error << t_path.string() << endl;
        return;
    }

    // read in cycel:
    uintmax_t size_left = file_size;
    bool read_error_again = false;
    while(size_left > 0) {
        // read to buffer
        unsigned to_read = m_rbuf_len < size_left ? m_rbuf_len : size_left;
        const size_t read = fread(m_rbuf, sizeof m_rbuf[0], to_read, file);
        // check length:
        if (to_read != read) {
            // we will accept one error...
            if (read_error_again) {
                cerr << progconst::read_less_again << t_path.string() << endl;
                break;
            }
            cerr << progconst::read_less << t_path.string() << endl;
            read_error_again = true;
        }
        // create segment, add to garb_collect collector, call sub job:
        unsigned index = 0;
        while (index < read){
            unsigned this_segment_length = 
                m_sbuf_len + m_overlap < (read - index) ? 
                m_sbuf_len + m_overlap : (read - index);
            unsigned this_segment_offset = 
                file_size - size_left + index;
            segment* seg = new segment
            {
                this_segment_offset, 
                this_segment_offset + this_segment_length == file_size ? true : false,
                this_segment_length, 
                &m_rbuf[index]
            };
            garb_collect.push_back(seg);
            call_sub_job(seg);
            index += this_segment_length;
        }
        size_left -= read;
    }
    fclose(file);
    // wait for sub-job threads to finnish with
    // job (segments) passed through call_sub_job():
    if (m_parallel) {
        wait_to_sub_finish();
    }
    // free memory:
    clear_collector();
    // process match(es) from this file:
    process_sub_results();
}

void FileRead::start()
{
    TaskParallelizer<string, fs::path, int, FileRead>* super_class = 
        (TaskParallelizer<string, fs::path, int, FileRead>*) get_super_class();
    fs::path path = super_class->next_job_argument();
    start(path);
}

// delete all results created by 
// sub-job classes and pushed to this class
// by calling put_sub_result(BCK &t_result)
inline void FileRead::clear_collector(){
    for (auto &elem : garb_collect) {
            delete elem;
    }
    garb_collect.clear();
}

void FileRead::process_sub_results()
{
    m_sub_job_results.sort();
    // file name:
    //string filename = m_file_path.filename().string();
    // full path: (TODO, add cmd switch)
    string filename = m_file_path.string();
    while (!m_sub_job_results.empty()) {
        const result* match = m_sub_job_results.pop_blocking();
        // <file>(<position>):<prefix>...<sufix> 
        cout << "<" << filename << ">(<" << match->offset << ">):";
        cout << "<" << match->prefix << ">...<" << match->sufix << ">\n";
        delete match;
    }
}