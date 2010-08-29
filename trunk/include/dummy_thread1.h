#ifndef DUMMY_THREAD1_H
#define DUMMY_THREAD1_H

#include "GRIProcessThread.h"

class DummyThread1 : public GRIProcessThread
{

public:
    DummyThread1(QObject* obj)
        : GRIProcessThread(obj) {
    }

    ~DummyThread1(){

    }

    void run() {
        for(int i = 0; i < 10; i++) {
            char test[] = "BBB";
            if(reg->writeMemory("ENERGY", 3, test)) {
                cout << "success" << endl;
            }

            QThread::yieldCurrentThread();
        }
    }
};

#endif // DUMMY_THREAD1_H
