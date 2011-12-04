#ifndef QVTKHELLOWORLDWIDGET_H
#define QVTKHELLOWORLDWIDGET_H


//////////////////////////////////////////////////////////////
//
//    --- QVTKHelloWorldWidget ---
//
// A QVTKWidget-derived class to test an interactive QVTKWidget.
// The widget simply displays "Hello World" and "QVTKWidget".
// The user can rotate the text by clicking and dragging the mouse.
//
// You can use it like any QWidget.  For example, it can be
// displayed on its own:
//
//     QVTKHelloWorldWidget *hellowidget = new QVTKHelloWorldWidget();
//     hellowidget->show();
//      ...
//     delete hellowidget;
//
// Or it can be placed inside another QWidget, such as a
// QMainWindow:
//
//     QMainWindow *win = new QMainWindow();
//     QVTKHelloWorldWidget *hellowidget = new QVTKHelloWorldWidget(win);
//     win->setCentralWidget(hellowidget);
//     win->show();
//      ...
//     delete win;  // hellowidget is a child of win, so it doesn't need to be deleted on its own
//
//////////////////////////////////////////////////////////////


#include <QVTKWidget.h>

#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkTextSource.h>
#include <vtkVectorText.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>


class QVTKHelloWorldWidget : public QVTKWidget
{
    Q_OBJECT

public:

    // Constructor/Destructor
    QVTKHelloWorldWidget(QWidget *parent = 0);
    ~QVTKHelloWorldWidget();

public slots:

protected:

protected slots:

private:

    // Drawing objects
    vtkTextSource *m_text;
    vtkVectorText *m_vectorText;
    vtkPolyDataMapper *m_textMapper;
    vtkPolyDataMapper *m_vectorTextMapper;
    vtkActor *m_textActor;
    vtkActor *m_vectorTextActor;
    vtkRenderer *m_renderer;
    vtkRenderWindow *m_renderWindow;

};

#endif // QVTKHELLOWORLDWIDGET_H
