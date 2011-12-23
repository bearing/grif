QT       += core network xml
QT       -= gui

# Change this variable to whatever your project name is
TARGET = networking

TEMPLATE = app

DEPENDPATH += ../../ . \
        ./src \
        ./include \
        ../../framework/source \
        ../../framework/include

INCLUDEPATH += ../../ . \
        ./include \
        ../../framework/include \
        ../../framework/source

include(../../framework/source/fsource.pri)
include(../../framework/include/finclude.pri)

# Your application files
HEADERS += \
    ./include/TcpAnalysisThread.h \
    ./include/TcpDAQThread.h
SOURCES += \
    ./src/main.cpp \
    ./src/TcpAnalysisThread.cpp \
    ./src/TcpDAQThread.cpp

# GRIFDIR is the head directory of GRIF on your machine
# ***You must set GRIFDIR as an environment variable***
GRIFDIR=$$(PWD)/../..

ROOTDIR=/Users/benson/Desktop/root_x86_64

# root headers
INCLUDEPATH += $$ROOTDIR/include

ROOTSYSLIB += $$ROOTDIR/lib
INCLUDEPATH += $$ROOTSYSLIB
LIBS += $$ROOTSYSLIB/libCint.so
LIBS += $$ROOTSYSLIB/libMatrix.so
LIBS += $$ROOTSYSLIB/libMathCore.so
LIBS += -L/$$ROOTSYSLIB
LIBS += -L$$ROOTSYS/lib -lCore -lHist

EXTDIR = $$GRIFDIR/external
INCLUDEPATH += EXTDIR

# run code generation
macx|unix {
    system(cd $$GRIFPROJECTDIR; ./setup.py)
}
win32 {
    system(cd $$GRIFPROJECTDIR; python setup.py)
}

QMAKE_CXXFLAGS += -D GRIF_CODE_GENERATION=1 -O3
