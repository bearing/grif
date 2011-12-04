#ifndef RUNWINDOW_H
#define RUNWINDOW_H

#include <QMainWindow>
#include <CCI2Run.h>
//#include "Windows.h"

namespace Ui {
    class RunWindow;
}

class RunWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RunWindow(QWidget *parent = 0);
    ~RunWindow();

private:

    void* usbptr;
    CCI2Run *cci2;
    Ui::RunWindow *ui;
private slots:
    void start();
    void stop();
};

#endif // RUNWINDOW_H
