#ifndef QVTKHISTOGRAMWIDGET_H
#define QVTKHISTOGRAMWIDGET_H

//////////////////////////////////////////////////////////////
//
//    --- QVTKHistogramWidget ---
//
// A QVTKWidget-derived class to display a single GRIHistogram.
// The user can interact with the histogram using the mouse,
// including dragging to pan, using the scroll button to zoom,
// and mousing over data to display the channel and data.
//
// You can use it like any QWidget.  For example, it can be
// displayed on its own:
//
//     QVTKHistogramWidget *histwidget = new QVTKHistogramWidget();
//     histwidget->setHist(A1->GetHistogram("ADCHist"));
//     histwidget->show();
//      ...
//     delete histwidget;
//
// Or it can be placed inside another QWidget, such as a
// QMainWindow:
//
//     QMainWindow *win = new QMainWindow();
//     QVTKHistogramWidget *histwidget = new QVTKHistogramWidget(win);
//     histwidget->setHist(A1->GetHistogram("ADCHist"));
//     win->setCentralWidget(histwidget);
//     win->show();
//      ...
//     delete win;  // histwidget is a child of win, so it doesn't need to be deleted on its own
//
//////////////////////////////////////////////////////////////

#include <vtkAxis.h>
#include <vtkChartXY.h>
#include <vtkColor.h>
#include <vtkContextView.h>
#include <vtkDoubleArray.h>
#include <vtkPlot.h>
#include <vtkPlotBar.h>
#include <vtkTable.h>

#include <QVTKWidget.h>
#include <QColor>
#include <QTimer>

#include "GRIHistogrammer.h"


class QVTKHistogramWidget : public QVTKWidget
{
    Q_OBJECT

public:

    // Constructor/Destructor
    QVTKHistogramWidget(QWidget *parent = 0);
    ~QVTKHistogramWidget();
    void setHist(GRIHistogrammer *grihist);
    GRIHistogrammer* getHist() { return m_grihist; };
    void setColor(QColor qcolor) { this->m_plotColor = qcolor; };
    double getChartMaximum() { return m_chartxy->GetAxis(0)->GetMaximum(); };
    void SetYScaleToLog(bool input) {this->m_logScale = input;
        this->m_chartxy->GetAxis(0)->SetTitle("countrate");};

public slots:
    void updateData();

protected:

protected slots:

private:

    // underlying histogram to display
    GRIHistogrammer *m_grihist;

    // Drawing objects
    vtkSmartPointer<vtkChartXY> m_chartxy;
    vtkSmartPointer<vtkContextView> m_contextView;
    vtkSmartPointer<vtkTable> m_table;
    vtkSmartPointer<vtkDoubleArray> m_arrX, m_arrY;
    vtkSmartPointer<vtkPlot> m_plot;

    // timer to update data
    QTimer* m_dataUpdateTimer;

    // misc
    QColor m_plotColor;
    bool m_logScale;

};

#endif // QVTKHISTOGRAMWIDGET_H
