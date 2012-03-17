+= core network xml
QT       -= gui

# Change this variable to whatever your project name is
TARGET = timeseries

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
    ./include/TSAnalysisThread.h \
    ./include/TSDAQThread.h
SOURCES += \
    ./src/main.cpp \
    ./src/TSAnalysisThread.cpp \
    ./src/TSDAQThread.cpp

QMAKE_CXXFLAGS += -D GRIF_CODE_GENERATION=1 -O3

# Top directory of GRIF on your machine
GRIFDIR=/Users/amidvidy/grif

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
ROOTDIR=/Users/amidvidy/root
INCLUDEPATH += $$ROOTDIR/include

# ROOT libraries
ROOTSYSLIB += $$ROOTDIR/lib
INCLUDEPATH += $$ROOTSYSLIB
LIBS += $$ROOTSYSLIB/libCint.so
LIBS += $$ROOTSYSLIB/libMatrix.so
LIBS += $$ROOTSYSLIB/libMathCore.so
LIBS += -L$$ROOTSYSLIB
LIBS += -L$$ROOTSYS/lib -lCore -lHist


CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DEFINES += GRIFPROJECTDIR=\"\\\"/home/ben/Documents/grif\\\"\"

SOURCES += \
    main.cpp
