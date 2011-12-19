QT       += core network xml
QT       -= gui

# Change this variable to whatever your project name is
TARGET = FallTesting

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
    ./include/SIMAnalysisThread.h \
    ./include/SIMDAQThread.h
SOURCES += \
    ./src/main.cpp \
    ./src/SIMAnalysisThread.cpp \
    ./src/SIMDAQThread.cpp

# root headers
ROOTDIR = /home/ben/Downloads/root
INCLUDEPATH += $$ROOTDIR/include

BOOSTDIR = /Users/benson/Desktop/grif/external
INCLUDEPATH += $$BOOSTDIR

ROOTSYSLIB += $$ROOTDIR/lib
INCLUDEPATH += $$ROOTSYSLIB
LIBS += $$ROOTSYSLIB/libCint.so
LIBS += $$ROOTSYSLIB/libMatrix.so
LIBS += $$ROOTSYSLIB/libMathCore.so
LIBS += -L/$$ROOTSYSLIB
LIBS += -L$$ROOTSYS/lib -lCore -lHist

# Master environment variable
# ****YOU MUST SET THIS VARIABLE****
GRIFDIR=/home/ben/Documents/grif
GRIFPROJECTDIR=$$GRIFDIR/examples/simulator

# run code generation
macx|unix {
    system(cd $$GRIFPROJECTDIR; ./setup.py)
}
win32 {
    system(cd $$GRIFPROJECTDIR; python setup.py)
}

QMAKE_CXXFLAGS += -D GRIF_CODE_GENERATION=1 -O3
