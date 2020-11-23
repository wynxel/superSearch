/*

                TODO
                
    

*/
#include "file_read.h"

FileRead::FileRead(const struct job_details t_jobs[], 
            const unsigned t_job_num, 
            TaskContainer* t_super_job_class) 
            : TaskParallelizer(t_jobs, t_job_num, t_super_job_class){};
FileRead::~FileRead(){};

void FileRead::start(fs::path &t_path)
{
    // TODO:
    /*
    vyrobit garbage collector (lebo bude new segment)
    vyrobit funkciu co deletne obsah garbage collectoru

    zisti velkost suboru, zisti velkost ram?
    alokuj si pole
        ak sa da na cely subor
        ak nie, tak nejaky max dlzku a budes robit v cykle (s presahom)
    otvor subor a citaj ho do pola (citaj po nejakych vhodnych blokoch. Velkost cache?)
    vytvaraj segmenty (este predtym kontrola, aky segment mam. Moze byt zly?)
        poporcuj pole (kedze segment ma pointer na char)
        mozno treba dat do structu segment length parameter (je dozveditelny, ale bol by tam vhodny)    
    pozor na vynimky
    volaj call sub job

    ...
    dalsie veci:
    volat garbage collector ked treba
    spracovavat navratovu hodnotu ked treba (ten "RESULT")
        vlastne nevieme ci ho bude pouzivat aj file_iterate
    */
    cout << t_path << "\n";
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