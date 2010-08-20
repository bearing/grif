#include "GRILoader.h"

GRILoader::GRILoader(GRIRegulator* regulator)
{
    this->regulator = regulator;
}

GRILoader::~GRILoader()
{

}

GRIProcessObj* GRILoader::load(string process_name, string xml_file)
{
    return NULL;
}
