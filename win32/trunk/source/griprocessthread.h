#ifndef GRIPROCESSTHREAD_H
#define GRIPROCESSTHREAD_H

#include <QObject>
#include <QThread>



using namespace std;

class GRIProcessThread : public QThread
{
    //Q_OBJECT
public:

    GRIProcessThread(QObject *obj);


    unsigned int getID();

private:
    unsigned int thread_id;
    static unsigned int counter;

};


#endif // GRIPROCESSTHREAD_H
