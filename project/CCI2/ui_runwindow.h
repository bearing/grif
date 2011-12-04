/********************************************************************************
** Form generated from reading UI file 'runwindow.ui'
**
** Created: Tue Oct 18 15:32:32 2011
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RUNWINDOW_H
#define UI_RUNWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QFormLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RunWindow
{
public:
    QWidget *centralwidget;
    QPushButton *startrun;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QSpinBox *peakingtime;
    QLabel *label;
    QLabel *label_2;
    QSpinBox *gaptime;
    QPushButton *pushButton;
    QCheckBox *checkBox;
    QPushButton *stoprun;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *RunWindow)
    {
        if (RunWindow->objectName().isEmpty())
            RunWindow->setObjectName(QString::fromUtf8("RunWindow"));
        RunWindow->resize(333, 216);
        centralwidget = new QWidget(RunWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        startrun = new QPushButton(centralwidget);
        startrun->setObjectName(QString::fromUtf8("startrun"));
        startrun->setGeometry(QRect(180, 30, 91, 24));
        formLayoutWidget = new QWidget(centralwidget);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(10, 40, 141, 61));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        peakingtime = new QSpinBox(formLayoutWidget);
        peakingtime->setObjectName(QString::fromUtf8("peakingtime"));
        peakingtime->setMinimum(1);
        peakingtime->setMaximum(80);
        peakingtime->setValue(10);

        formLayout->setWidget(0, QFormLayout::FieldRole, peakingtime);

        label = new QLabel(formLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        label_2 = new QLabel(formLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        gaptime = new QSpinBox(formLayoutWidget);
        gaptime->setObjectName(QString::fromUtf8("gaptime"));
        gaptime->setMinimum(1);
        gaptime->setValue(5);

        formLayout->setWidget(1, QFormLayout::FieldRole, gaptime);

        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(140, 140, 141, 24));
        checkBox = new QCheckBox(centralwidget);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(50, 140, 79, 21));
        stoprun = new QPushButton(centralwidget);
        stoprun->setObjectName(QString::fromUtf8("stoprun"));
        stoprun->setGeometry(QRect(180, 50, 91, 24));
        RunWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(RunWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 333, 21));
        RunWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(RunWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        RunWindow->setStatusBar(statusbar);

        retranslateUi(RunWindow);

        QMetaObject::connectSlotsByName(RunWindow);
    } // setupUi

    void retranslateUi(QMainWindow *RunWindow)
    {
        RunWindow->setWindowTitle(QApplication::translate("RunWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        startrun->setText(QApplication::translate("RunWindow", "Start Run", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("RunWindow", "peaking time", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("RunWindow", "gap time", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("RunWindow", "choose filename", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("RunWindow", "Save File", 0, QApplication::UnicodeUTF8));
        stoprun->setText(QApplication::translate("RunWindow", "Stop Run", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RunWindow: public Ui_RunWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RUNWINDOW_H
