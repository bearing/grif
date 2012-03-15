
QT       += core network qt3support xml
QT       += gui

TARGET = GRIHistWidgetTest
CONFIG   += console
TEMPLATE = app

DEPENDPATH += ../../../ . \
        ../../../application/daq/simulator/trunk/source \
        ../../../application/daq/simulator/trunk/include \
        ../../../framework/trunk/source \
        ../../../framework/trunk/include \
        ../../../application/gui/grihistwidget/trunk/include \
        ../../../application/gui/grihistwidget/trunk/source

INCLUDEPATH += ../../../ . \
        ../../../application/analysis/simulator/trunk/include \
        ../../../application/daq/simulator/trunk/include \
        ../../../framework/trunk/include \
        ../../../framework/trunk/source \
        ../../../application/gui/grihistwidget/trunk/include

include(../../../framework/trunk/source/fsource.pri)
include(../../../framework/trunk/include/finclude.pri)
include(../../../application/daq/simulator/trunk/source/ssource.pri)
include(../../../application/daq/simulator/trunk/include/sinclude.pri)

# Source file and headers
SOURCES += main.cpp \
    ./SIMMCAnalysisThread.cpp \
    ./GRIUserLoader.cpp \
    ../../../application/gui/grihistwidget/trunk/source/GRIHist1DWidget.cpp \
    ../../../application/gui/grihistwidget/trunk/source/GRIHist1DGridWidget.cpp \
    ../../../application/gui/grihistwidget/trunk/source/GRIHist1DGridScrollWidget.cpp \
    ../../../application/gui/grihistwidget/trunk/source/GRIHist2DWidget.cpp

HEADERS += \
    ./SIMMCAnalysisThread.h \
    ./GRIUserProcesses.h \
    ./GRIDataDefines.h \
    ../../../application/gui/grihistwidget/trunk/include/GRIHist1DWidget.h \
    ../../../application/gui/grihistwidget/trunk/include/GRIHist1DGridWidget.h \
    ../../../application/gui/grihistwidget/trunk/include/GRIHist1DGridScrollWidget.h \
    ../../../application/gui/grihistwidget/trunk/include/GRIHist2DWidget.h

ROOTSYS=/usr/local/root
GRIFDIR=$$HOME/Projects/GRIF/grif
GRIFPROJECTDIR=$$GRIFDIR/project/bandstra/Hist2D

INCLUDEPATH += $$ROOTSYS/include
ROOTSYSLIB += $$ROOTSYS/lib

# load in all ROOT libraries
LIBS += -L$$ROOTSYSLIB
LIBS += -L$$ROOTSYSLIB -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lRint -lPostscript
LIBS += -L$$ROOTSYSLIB -pthread -ldl -rdynamic -lm -lMatrix -lPhysics -lTree
LIBS += -L$$ROOTSYSLIB -lMathCore -lThread -lz -lGui -lpthread -lGpad -lSpectrum


