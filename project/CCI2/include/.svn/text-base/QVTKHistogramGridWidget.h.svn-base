#ifndef QVTKHISTOGRAMGRIDWIDGET_H
#define QVTKHISTOGRAMGRIDWIDGET_H


//////////////////////////////////////////////////////////////
//
//    --- QVTKHistogramGridWidget ---
//
// A QVTKWidget-derived class to display multiple GRIHistograms
// in a grid layout.  Each histogram is a QVTKHistogramWidget
// that the user can interact with.
//
// Histograms are stored in a 1D list.  Call addHist to add
// a histogram to that list.  The grid is automatically set so that
// the number of rows and columns is the same; to override, call
// setGridNx (sets # of columns) or setGridNy (sets # of rows).
// To access the QVTKHistogramWidget for a certain GRIHistogrammer
// (e.g., in order to set the color), call histWidget.
//
// You can use it like any QWidget.  For example, it can be
// displayed on its own:
//
//     QVTKHistogramGridWidget *histgrid = new QVTKHistogramGridWidget();
//     histgrid->addHist(A1->GetHistogram("ADC Channel 0"));
//     histgrid->addHist(A1->GetHistogram("ADC Channel 1"));
//     histgrid->addHist(A1->GetHistogram("ADC Channel 2"));
//     histgrid->histWidget(A1->GetHistogram("ADC Channel 0"))->setColor(QColor(255,0,0));
//     histgrid->setGridNx(1);    // optional; setGridNx forces the number of grid columns
//     histgrid->resize(100,300);
//     histgrid->show();
//      ...
//     delete histgrid;
//
// Or it can be placed inside another QWidget, such as a
// QMainWindow:
//
//     QMainWindow *win = new QMainWindow();
//     QVTKHistogramGridWidget *histgrid = new QVTKHistogramGridWidget(win);
//     histgrid->addHist(A1->GetHistogram("ADC Channel 0"));
//     histgrid->addHist(A1->GetHistogram("ADC Channel 1"));
//     histgrid->addHist(A1->GetHistogram("ADC Channel 2"));
//     histgrid->histWidget(A1->GetHistogram("ADC Channel 0"))->setColor(QColor(255,0,0));
//     histgrid->setGridNy(3);    // optional; setGridNy forces the number of grid rows
//     win->setCentralWidget(histgrid);
//     win->resize(100,300);
//     win->show();
//      ...
//     delete win;  // histgrid is a child of win, so it doesn't need to be deleted on its own
//
//////////////////////////////////////////////////////////////


#include "QVTKHistogramWidget.h"
#include "GRIHistogrammer.h"

#include <QMainWindow>
#include <QWidget>
#include <QFrame>
#include <QGridLayout>


class QVTKHistogramGridWidget : public QFrame
{
    Q_OBJECT

public:

    // Constructor/Destructor
    QVTKHistogramGridWidget(QWidget *parent = 0);
    ~QVTKHistogramGridWidget();

    void addHist(GRIHistogrammer *h);
    void setGridNx(int nx);
    void setGridNy(int ny);
    QVTKHistogramWidget* histWidget(GRIHistogrammer *h);

public slots:

protected:

protected slots:

private:

    void setGrid();
    void clearGrid();
    bool histIsPresent(GRIHistogrammer *h);
    int histWidgetIndex(GRIHistogrammer *h);

    QVector<QVTKHistogramWidget *> m_histWidget;
    QGridLayout *m_gridLayout;

};

#endif // QVTKHISTOGRAMGRIDWIDGET_H
