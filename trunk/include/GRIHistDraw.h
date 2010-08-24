#ifndef GRIHISTDRAW_H
#define GRIHISTDRAW_H

#endif // GRIHISTDRAW_H

using namespace std;
#include "GRIHistogrammer.h"
#include "TQtWidget.h"
#include <QVector>


class GRIHistDraw : public QObject
{

    Q_OBJECT

public:
    GRIHistDraw();
    ~GRIHistDraw();
    int currentSize;
    QVector<TQtWidget *> histWidg;

protected slots:
    virtual void getAndDrawHist(GRIHistogrammer *);

};
