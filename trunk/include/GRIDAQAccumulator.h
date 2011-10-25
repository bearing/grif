#ifndef GRIDAQACCUMULATOR_H
#define GRIDAQACCUMULATOR_H

#include "GRIAccumBuff.h"
#include "GRIDAQAccumNode.h"
#include <QString>

template <class T>
class GRIDAQAccumulator : public GRIDAQAccumNode {
  public:
    GRIDAQAccumulator(QString  bname, qint64 ticks, int NBuff, int AccumMsec) {
      BufferName = bname;
      ticksPerSecond = ticks;
      NAccumBuff = NBuff;
      AccumulationTime = AccumMsec*1E-3*ticks;  //Accumulation Time in ticks

      for (int i=0; i < NBuff; i++) {
        GRIAccumBuff<T>* b = new GRIAccumBuff<T>();
        buff_.push_back(b);
      }
    }

    ~GRIDAQAccumulator() {}

    void ResetAccumBuffs(qint64 t_0) {
        t_0 = 0; //reduce compiler warnings

        // NOTE: need typename when defining a template iterator...
        typename list<GRIAccumBuff<T>*>::iterator  buff_it;

        // reset all times in the order of the buffers
        int buffcnt = 0;
        qint64 t1_,t2_;

        for (buff_it = buff_.begin(); buff_it != buff_.end(); buff_it++) {
            t1_ = ts0 + buffcnt*AccumulationTime;
            t2_ = t1_ + AccumulationTime;
            GRIAccumBuff<T>* b = *buff_it;
            b->ResetBuffer(t1_,t2_);

            // First buffer in list is on the bubble
            if(buffcnt == 0)
                b->set_bubble(true);
            else
                b->set_bubble(false);

            // Last buffer in list is leading Edge
            if(buffcnt == NAccumBuff-1)
                b->set_leading_edge(true);
            else
                b->set_leading_edge(false);

            buffcnt+=1;
        }
    }

    void Accumulate(int numel, T data[], qint64 timestamps[],bool runflag) {
        // Trigger write when timestamps are greater than any window
        // Use bubble member to determine who is next...

        // NOTE: need typename when defining a template iterator...
        typename list<GRIAccumBuff<T>*>::iterator  buff_it;

        bool FlushFlag = !runflag;

        bool BubbleTrigger = false;
        int BubbleWriteNum = 0;
        qint64 BubbleDT = 0;

        if(!FlushFlag) {
            for (buff_it = buff_.begin(); buff_it != buff_.end(); buff_it++) {
                GRIAccumBuff<T>* b = *buff_it;
                // Buffer Data and sense the bubble
                BubbleDT = (b->BufferData(numel, timestamps,data));
                if (BubbleDT) {
                    BubbleTrigger = true;
                    // TODO(arbenson): BubbleWriteNum is used when data is missing for accumulation times
                    BubbleWriteNum = (int)((BubbleDT)/AccumulationTime + 1);
                }
            }
        }

        if (BubbleTrigger) {
            for (int i=0; i < BubbleWriteNum; i++) {
                // We want to go through the buffers again and
                // 1) Write the bubble buffer to the mm
                // 2) Reset the bubble buffer to the leading edge
                // 3) Re-buffer the data for the new buffer
                // 4) Set the new bubble

                int nbuffcnt = 0;
                bool NewBubble = false;
                bool NewBubbleSet = false;

                for(buff_it = buff_.begin(); buff_it != buff_.end(); buff_it++) {
                    nbuffcnt = nbuffcnt + 1;
                    GRIAccumBuff<T>* b = *buff_it;
                    //cout << "IN: Buffer " << nbuffcnt << ": Bubble: " << b->get_bubble() << " Leading Edge: " << b->get_leading_edge() << endl;

                    if (NewBubble) {
                        b->set_bubble(true);
                        NewBubbleSet = true;
                        NewBubble = false;
                    }

                    if (b->get_leading_edge())
                      b->set_leading_edge(false);

                    // Check for bubble
                    if (b->get_bubble() && !NewBubbleSet) {
                        // Write Bubble Accumulator to mm
                        cout << QTime::currentTime().toString("hh:mm:ss.zzz").toStdString().c_str() << ": " <<
                                GetBufferName().toStdString().c_str() << "(" << nbuffcnt << ") accumulator writing "
                                << b->GetDataSize() << " events. Timestamp = " << (double)timestamps[0]/(double)ticksPerSecond << endl;
                        T* da = b->DataArray();

                        pDAQ->writeMemory(pDAQ->get_name(),GetBufferName(),b->GetDataSize(),da);
                        delete [] da;

                        NewBubble = true;  // Sets up next buffer to be the bubble
                        b->set_bubble(false);
                        b->ResetBuffer(b->get_t1() + AccumulationTime*NAccumBuff,
                                       b->get_t2() + AccumulationTime*NAccumBuff);
                        b->set_leading_edge(true);
                        b->BufferData(numel, timestamps,data);
                    }
                }

                // If we are at the end of the buffer list and need a new bubble
                if (!NewBubbleSet) {
                    GRIAccumBuff<T>* b2 = buff_.front();
                    b2->set_bubble(true);   
                }
            }
        }
    }


 void FlushBuffers() {
            typename list<GRIAccumBuff<T>*>::iterator  buff_it;

            // Loop over all buffers and write the bubbles out...
            for (int i=0; i<NAccumBuff; i++) {
                bool NewBubble = false;
                bool NewBubbleSet = false;
                cout << "Flushing AccumBuff Iteration " << i << endl;
                int buffctr = 0;
                for (buff_it = buff_.begin(); buff_it != buff_.end(); buff_it++) {
                    buffctr = buffctr + 1;
                    GRIAccumBuff<T>* b = *buff_it;

                    if (NewBubble) {
                        b->set_bubble(true);
                        NewBubbleSet = true;
                        NewBubble = false;
                    }

                    if(b->get_leading_edge())
                        b->set_leading_edge(false);

                    // Check for bubble
                    if (b->get_bubble() && !NewBubbleSet) {
                        // Write Bubble Accumulator to mm
                        cout << "FLUSH (" << buffctr << "): " 
                             << QTime::currentTime().toString("hh:mm:ss.zzz").toStdString().c_str() << ": " 
                             << GetBufferName().toStdString().c_str() << " accumulator writing "  
			     << b->GetDataSize() << " events." << endl;
                        T* da = b->DataArray();

                        pDAQ->writeMemory(pDAQ->get_name(),GetBufferName(),b->GetDataSize(),da);
                        delete [] da;

                        NewBubble = true;  // Sets up next buffer to be the bubble
                        b->set_bubble(false);
                        b->ResetBuffer(b->get_t1() + AccumulationTime*NAccumBuff,
                                       b->get_t2() + AccumulationTime*NAccumBuff);
                        b->set_leading_edge(true);
                    }
                }

                // If we are at the end of the buffer list and need a new bubble
                if (!NewBubbleSet) {
                    GRIAccumBuff<T>* b2 = buff_.front();
                    b2->set_bubble(true);
                }
            }
     }

private:
    list<GRIAccumBuff<T>*> buff_;
};


#endif // GRIDAQACCUMULATOR_H
