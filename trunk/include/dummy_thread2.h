#ifndef DUMMY_THREAD2_H
#define DUMMY_THREAD2_H

#include "GRIProcessThread.h"
#include "GRIRegulator.h"

class DummyThread2 : public GRIProcessThread
{

public:
    DummyThread2(QObject* obj)
        : GRIProcessThread(obj) {

    }

    ~DummyThread2(){

    }

    void run() {
        for(int i = 0; i < 8; i++) {
            cout << reg->readMemory("ENERGY") << endl;
            QThread::yieldCurrentThread();
        }
    }
};

#endif // DUMMY_THREAD1_H
