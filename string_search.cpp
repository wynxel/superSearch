/*

                TODO
                
    

*/
#include "string_search.h"

StringSearch::StringSearch(const struct job_details t_jobs[], 
            const unsigned t_job_num, 
            TaskContainer* t_super_job_class, const int t_id) 
            : TaskParallelizer(t_jobs, t_job_num, t_super_job_class, t_id){};

StringSearch::~StringSearch(){};

void StringSearch::start(segment* &t_path)
{
    /*
        vyhladaj v segmente
        pre kazdy vysky vytvor segment (alebo urob iny struct na to, 
        mozno je segment struct na to zbytocny)
        pridaj struct do resultu super triedy
        ku koncu dohladaj aj prefixy (+v zadani je nieco s \t \n ...)

        vyries otazku kto ma na zodpovednosti delete tych vysledkov pushnutych
         do super triedy
    */ 
    cout << "zeby sa nepovedalo " << t_path->length << endl;
    return;
}

void StringSearch::start()
{
    TaskParallelizer<fs::path, segment*, segment*, StringSearch>* super_class = 
        (TaskParallelizer<fs::path, segment*, segment*, StringSearch>*) get_super_class();
    segment* next_segm = super_class->next_job_argument();
    start(next_segm);
}

void StringSearch::process_sub_results()
{
    return;
}