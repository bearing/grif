#include "gri_Loader.h"

GRILoader::GRILoader(GRIRegulator* regulator)
{
    this->regulator = regulator;

}

GRILoader::~GRILoader()
{

}

list<GRIProcessThread*>* GRILoader::initProcessThreads(list<string> names, list<string> filepaths)
{
    // create a process thread list
    list<GRIProcessThread*>* processList = new list<GRIProcessThread*>;

    // create a process thread variable
    GRIProcessThread* p;

    // for however many process threads there are
    for(int i = 0; i < names.size(); i++)
    {
        // load the process thread
        p = load(names.front(), filepaths.front());

        filepaths.pop_front();
        names.pop_front();

        // add to list
        processList->push_front(p);
    }

    // return list
    return processList;
}

GRIProcessThread* GRILoader::load(string process_name, string xml_file)
{
    GRIProcessThread* p;
    // sadly, you can't use strings in c++ switch statements
    if(process_name == "hemi")   {
//        for example
//        p = new HemiDaq();
    }

    return p;
}
