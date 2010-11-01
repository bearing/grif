#include "GRITrace.h"

template <class T> GRITrace<T>::GRITrace()
{
    OffsetTimeData = false;
}

template <class T> GRITrace<T>::~GRITrace()
{

}


template <class T> void GRITrace<T>::AddData(T data)
{


}

template <class T> void GRITrace<T>::AddData(QList<T> data)
{


}


template <class T> void GRITrace<T>::AddData(QList<T> data, QList<double> tdata)
{


}


template <class T> QList<QTime> GRITrace<T>::GetTData()
{

}

template <class T> QList<T> GRITrace<T>::GetYData()
{

}

template <class T> void GRITrace<T>::OffsetTime(bool yn)
{

    this->OffsetTimeData = yn;
}
