#include "griprocessthread.h"



unsigned int GRIProcessThread::counter = 0;

GRIProcessThread::GRIProcessThread(QObject *obj)
    : QThread(obj)
{
    thread_id = GRIProcessThread::counter++;
    //curr_thread = NULL;
}


unsigned int GRIProcessThread::getID()
{
    return thread_id;
}


