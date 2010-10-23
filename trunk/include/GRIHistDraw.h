#ifndef GRIHISTDRAW_H
#define GRIHISTDRAW_H

using namespace std;
#include "GRIHistogrammer.h"
#include "TQtWidget.h"
#include <QVector>
#include <QObject>

class GRIHistDraw : public QObject
{
    Q_OBJECT

public:
    GRIHistDraw();
    ~GRIHistDraw();
    int currentSize;
    QVector<TQtWidget *> histWidg;

public slots:
    virtual void getAndDrawHist(GRIHistogrammer *myHist);
};


#endif // GRIHISTDRAW_H
