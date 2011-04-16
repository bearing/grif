#ifndef GRIDAQACCUMULATOR_H
#define GRIDAQACCUMULATOR_H

#include "GRIObject.h"
#include "GRIAccumBuff.h"
#include "GRIDAQAccumNode.h"
#include <QString>
#include <QDateTime>
#include <QVector>
#include <QTimerEvent>


template <class T>
        class GRIDAQAccumulator : public GRIDAQAccumNode
{

public:
    GRIDAQAccumulator(QString  bname, qint64 ticks, int NBuff, int AccumMsec)
    {
        this->BufferName = bname;
        this->ticksPerSecond = ticks;
        this->NAccumBuff = NBuff;
        this->AccumulationTime = AccumMsec*1E-3*ticks;  //Accumulation Time in ticks


        for(int i=0; i < NBuff; i++)
        {
            GRIAccumBuff<T>* b = new GRIAccumBuff<T>();
            buff.push_back(b);
        }

        //        cout << "Constructed GRIDAQAccumulator for " << bname.toStdString().c_str() << endl;
        //        CommitLog(LOG_VERBOSE);
        //        timer = new QTimer(this);
        //        connect(timer, SIGNAL(timeout()), this, SLOT(GRIDAQAccumulationTimer()));

    }

    ~GRIDAQAccumulator()
    {

    }

    void ResetAccumBuffs(qint64 t_0)
    {
        t_0 = 0; //reduce compiler warnings

        // NOTE: need typename when defining a template iterator...
        typename list<GRIAccumBuff<T>*>::iterator  buff_it;

        // reset all times in the order of the buffers
        int buffcnt = 0;
        qint64 t1_,t2_;

        for(buff_it = buff.begin(); buff_it != buff.end(); buff_it++)
        {
            t1_ = this->ts0 + buffcnt*this->AccumulationTime;
            t2_ = t1_ + this->AccumulationTime;
            GRIAccumBuff<T>* b = *buff_it;
            b->ResetBuffer(t1_,t2_);

            // First buffer in list is on the bubble
            if(buffcnt == 0)
                b->SetBubble(true);
            else
                b->SetBubble(false);

            // Last buffer in list is leading Edge
            if(buffcnt == this->NAccumBuff-1)
                b->SetLeadingEdge(true);
            else
                b->SetLeadingEdge(false);

            buffcnt+=1;
        }



    }

    void Accumulate(int numel, T data[], qint64 timestamps[],bool runflag){

        // Trigger write when timestamps are greater than any window
        // Use bubble member to determine who is next...

        // NOTE: need typename when defining a template iterator...
        typename list<GRIAccumBuff<T>*>::iterator  buff_it;


        bool FlushFlag = !runflag;

        //cout << "Accumulate (" << numel << ")" << endl;

        bool BubbleTrigger = false;
        int BubbleWriteNum = 0;
        qint64 BubbleDT = 0;

        if(!FlushFlag){
            for(buff_it = buff.begin(); buff_it != buff.end(); buff_it++)
            {

                GRIAccumBuff<T>* b = *buff_it;
                // Buffer Data and sense the bubble
                BubbleDT = (b->BufferData(numel, timestamps,data));
                if(BubbleDT)
                {
                    BubbleTrigger = true;
                    // BubbleWriteNum is used when data is missing for accumulation times
                    BubbleWriteNum = (int)((BubbleDT)/this->AccumulationTime + 1);
                }

            }


        }


        if(BubbleTrigger){

            for(int i=0; i < BubbleWriteNum; i++){
                // We want to go through the buffers again and
                // 1) Write the bubble buffer to the mm
                // 2) Reset the bubble buffer to the leading edge
                // 3) Re-buffer the data for the new buffer
                // 4) Set the new bubble

                int nbuffcnt = 0;
                bool NewBubble = false;
                bool NewBubbleSet = false;

                for(buff_it = buff.begin(); buff_it != buff.end(); buff_it++)
                {
                    nbuffcnt = nbuffcnt + 1;

                    GRIAccumBuff<T>* b = *buff_it;
                    //cout << "IN: Buffer " << nbuffcnt << ": Bubble: " << b->isBubble() << " Leading Edge: " << b->isLeadingEdge() << endl;

                    if(NewBubble) {
                        b->SetBubble(true);
                        NewBubbleSet = true;
                        NewBubble = false;
                    }

                    if(b->isLeadingEdge())
                        b->SetLeadingEdge(false);

                    // Check for bubble
                    if(b->isBubble() && !NewBubbleSet){

                        // Write Bubble Accumulator to mm
                        cout << QTime::currentTime().toString("hh:mm:ss.zzz").toStdString().c_str() << ": " <<
                                this->GetBufferName().toStdString().c_str() << "(" << nbuffcnt << ") accumulator writing "
                                << b->GetDataSize() << " events. Timestamp = " << (double)timestamps[0]/(double)this->ticksPerSecond << endl;
                        T* da = b->DataArray();

                        //                     for(int i=0; i<b->GetDataSize(); i++) cout << da[i] << " ";
                        //                     cout << endl;

                        pDAQ->writeMemory(pDAQ->get_name(),this->GetBufferName(),b->GetDataSize(),da);
                        delete [] da;

                        NewBubble = true;  // Sets up next buffer to be the bubble
                        b->SetBubble(false);
                        b->ResetBuffer(b->GetT1() + this->AccumulationTime*this->NAccumBuff,
                                       b->GetT2() + this->AccumulationTime*this->NAccumBuff);
                        b->SetLeadingEdge(true);
                        b->BufferData(numel, timestamps,data);
                    }
                    //cout << "OUT: Buffer " << nbuffcnt << ": Bubble: " << b->isBubble() << " Leading Edge: " << b->isLeadingEdge() << endl;
                }

                // If we are at the end of the buffer list and need a new bubble
                if(!NewBubbleSet){
                    GRIAccumBuff<T>* b2 = buff.front();
                    b2->SetBubble(true);
                    
                }

            }
        }
    }


 void FlushBuffers(){

            typename list<GRIAccumBuff<T>*>::iterator  buff_it;



            // Loop over all buffers and write the bubbles out...
            for(int i=0; i<this->NAccumBuff; i++){
                bool NewBubble = false;
                bool NewBubbleSet = false;
                cout << "Flushing AccumBuff Iteration " << i << endl;
                int buffctr = 0;
                for(buff_it = buff.begin(); buff_it != buff.end(); buff_it++)
                {
                    buffctr = buffctr + 1;
                    GRIAccumBuff<T>* b = *buff_it;

                    if(NewBubble) {
                        b->SetBubble(true);
                        NewBubbleSet = true;
                        NewBubble = false;
                    }

                    if(b->isLeadingEdge())
                        b->SetLeadingEdge(false);

                    // Check for bubble
                    if(b->isBubble() && !NewBubbleSet){

                        // Write Bubble Accumulator to mm
                        cout << "FLUSH (" << buffctr << "): " << QTime::currentTime().toString("hh:mm:ss.zzz").toStdString().c_str() << ": " <<
                                this->GetBufferName().toStdString().c_str() << " accumulator writing "  <<
                                b->GetDataSize() << " events." << endl;
                        T* da = b->DataArray();

                        //                     for(int i=0; i<b->GetDataSize(); i++) cout << da[i] << " ";
                        //                     cout << endl;


                        pDAQ->writeMemory(pDAQ->get_name(),this->GetBufferName(),b->GetDataSize(),da);
                        delete [] da;

                        NewBubble = true;  // Sets up next buffer to be the bubble
                        b->SetBubble(false);
                        b->ResetBuffer(b->GetT1() + this->AccumulationTime*this->NAccumBuff,
                                       b->GetT2() + this->AccumulationTime*this->NAccumBuff);
                        b->SetLeadingEdge(true);

                    }

                }

                // If we are at the end of the buffer list and need a new bubble
                if(!NewBubbleSet){
                    GRIAccumBuff<T>* b2 = buff.front();
                    b2->SetBubble(true);
                }
            }

 }



    void GRIDAQAccumulationTimer()
    {
        // Callback function for timer event
        QDateTime t = QDateTime::currentDateTime();
        //        cout << "Accumulation Timer: (" << this->GetBufferName().toStdString().c_str()
        //                << ") -- " << t.toString("dd.MM.yyyy hh:mm:ss.zzz").toStdString().c_str() << endl;

    }
private:

    list<GRIAccumBuff<T>*> buff;

};


#endif // GRIDAQACCUMULATOR_H
