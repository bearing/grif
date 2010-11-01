#ifndef GRITRACE_H
#define GRITRACE_H

#include <QList>
#include <QTime>

template <class T>
class GRITrace
{

public:
    GRITrace();
    ~GRITrace();
    void ClearData();
    void AddData(T ydata);
    void AddData(QList<T> ydata);
    void AddData(QList<T> ydata, QList<double> tdata);
    void OffsetTime(bool yn);
    QList<QTime> GetTData();
    QList<T> GetYData();

private:
    QList<T> tdata;
    QList<T> ydata;
    bool OffsetTimeData;



};




#endif // GRITRACE_H
