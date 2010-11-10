#ifndef GRIACCUMBUFF_H
#define GRIACCUMBUFF_H

#include <QVector>
#include <QDateTime>

template <class T>
class GRIAccumBuff
{

public:
    GRIAccumBuff()
    {
        bubble = false;
        LeadingEdge = false;
    }
    ~GRIAccumBuff(){}
    bool BufferData(int numel, qint64 t[], T d[])
    {
        bool trigger = false;

        for(int i=0; i<numel; i++)
        {
            if(t[i]>=t1 && t[i]<t2)
            {
                time.append(t[i]);
                data.append(d[i]);
            }

            // trigger on times greater than leading edge buffer
            if(t[i] >= t2 && LeadingEdge)
                trigger = true;

        }

        return trigger;
    }

    void ResetBuffer(qint64 t1_, qint64 t2_)
    {
        t1 = t1_;
        t2= t2_;
        data.clear();
        time.clear();
    }

    bool isBubble(){return bubble;}
    void SetBubble(bool tf){bubble = tf;}
    void SetLeadingEdge(bool tf){LeadingEdge = tf;}
    bool isLeadingEdge(){return LeadingEdge;}

    T* DataArray(){
        T* a = new T[data.size()];  //ToDo: Is this getting freed somewhere in the mm???

       // typename QList<T>::iterator data_it;
       // int ncnt = 0;

        for(int i=0; i < data.size(); i++)
        {
            a[i] = data[i];
        }
        return a;
    }

    int GetDataSize(){return data.size();}

    qint64 GetT1(){return t1;}
    qint64 GetT2(){return t2;}

private:
    QList<T> data;  // list mode data
    QList<qint64> time;  // list mode times
    qint64 t1,t2;  // Timestamp boundaries of this buffer
    bool bubble;  // Last buffer in times
    bool LeadingEdge;  // First Buffer in times

    T* dArray;

};


#endif // GRIACCUMBUFF_H
