#ifndef GRIDAQACCUMNODE_H
#define GRIDAQACCUMNODE_H

#include <QString>
#include <QTimer>
#include "GRIObject.h"
#include "GRIProcessThread.h"
#include "GRIDAQAccumulator.h"

// Abstract Container for GRIDAQAccumulators of different types

class GRIDAQAccumNode: public GRIObject
{

public:

    virtual void GRIDAQAccumulationTimer()=0;
    virtual void ResetAccumulators(qint64 t_0)=0;

    //These are placeholders for supported data types
    virtual void Accumulate(int numel, double data[], qint64 timestamps[]){} // placeholders
    virtual void Accumulate(int numel, float data[], qint64 timestamps[]){}
    virtual void Accumulate(int numel, int data[], qint64 timestamps[]){}
    virtual void Accumulate(int numel, qint64 data[], qint64 timestamps[]){}
    virtual void Accumulate(int numel, long data[], qint64 timestamps[]){}
    virtual void Accumulate(int numel, short data[], qint64 timestamps[]){}
    virtual void Accumulate(int numel, char data[], qint64 timestamps[]){}
    virtual void Accumulate(int numel, unsigned char data[], qint64 timestamps[]){}
    virtual void Accumulate(int numel, bool data[], qint64 timestamps[]){}

private:
    void timerEvent()
    {   GRIDAQAccumulationTimer(); }

public:
    QString GetBufferName(){return BufferName;}
    void SetBufferName(QString bname){BufferName = bname;}
    int GetAccumulationTime(){return AccumulationTime;}
    void SetAccumulationTime(int t){AccumulationTime = t;}
    int GetNAccumBuff(){return NAccumBuff;}
    void SetNAccumBuff(int n){NAccumBuff = n;}
    qint64 GetTicksPerSecond(){return ticksPerSecond;}
    void SetTicksPerSecond(qint64 ticks){ticksPerSecond = ticks;}
    bool isRunning(){return running;}
    void SetDAQThreadObject(GRIProcessThread* pDT){pDAQ = pDT;}
    void Initialize(QDateTime tst, qint64 t_0){
        this->InitializeTime(tst,t_0,ticksPerSecond);
    }

    void InitializeTime(qint64 timestamp)
    {
    }

    void InitializeTime(QDateTime tst, qint64 timestamp, qint64 ticks)
    {

        this->t0 = tst.time();
        this->ts0 = timestamp;
        this->ticksPerSecond = ticks;
        this->running = true;
        ResetAccumulators(timestamp);

    }


protected:
    QString BufferName;
    QTime t0;
    qint64 AccumulationTime;  // Accumulation Time in ticks
    int NAccumBuff;  //Number of buffers
    int timerID;
    qint64 ts0;
    qint64 ticksPerSecond;
    bool running;
    QTimer *timer;
    GRIProcessThread* pDAQ;
};


#endif // GRIDAQACCUMNODE_H
