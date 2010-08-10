#include "griprocessthread.h"



unsigned int GRIProcessThread::counter = 0;

GRIProcessThread::GRIProcessThread(QObject *obj)
    : QThread(obj)
{
    thread_id = GRIProcessThread::counter++;

}


unsigned int GRIProcessThread::getID()
{
    return thread_id;
}


