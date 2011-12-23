QT       += core network xml
QT       -= gui

# Change this variable to whatever your project name is
TARGET = FallTest

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

QMAKE_CXXFLAGS += -D GRIF_CODE_GENERATION=1 -O3

# Top directory of GRIF on your machine
GRIFDIR=$$(GRIF)

# run code generation
GRIFPROJECTDIR = $$GRIFDIR/examples/simulator
macx|unix {
    system(cd $$GRIFPROJECTDIR; ./setup.py)
}
win32 {
    system(cd $$GRIFPROJECTDIR; python setup.py)
}

QMAKE_CXXFLAGS += -D GRIF_CODE_GENERATION=1

DEFINES += GRIFPROJECTDIR=$${GRIFPROJECTDIR}

# External libraries
INCLUDEPATH += $$GRIFDIR/external

# ROOT headers
ROOTDIR=$$(ROOT)
INCLUDEPATH += $$ROOTDIR/include

# ROOT libraries
ROOTSYSLIB += $$ROOTDIR/lib
INCLUDEPATH += $$ROOTSYSLIB
LIBS += $$ROOTSYSLIB/libCint.so
LIBS += $$ROOTSYSLIB/libMatrix.so
LIBS += $$ROOTSYSLIB/libMathCore.so
LIBS += -L$$ROOTSYSLIB
LIBS += -L$$ROOTSYS/lib -lCore -lHist

