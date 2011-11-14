QT       += core network qt3support xml
QT       -= gui

# Change this variable to whatever your project name is
TARGET = FallTesting

TEMPLATE = app

DEPENDPATH += ../../ . \
        ./src \
        ./include \
        ../../source \
        ../../include

INCLUDEPATH += ../../ . \
        ./include \
        ../../include \
        ../../source

include(../../source/fsource.pri)
include(../../include/finclude.pri)

# Your application files
HEADERS += \
    ./include/SIMAnalysisThread.h \
    ./include/SIMDAQThread.h
SOURCES += \
    ./src/main.cpp \
    ./src/SIMAnalysisThread.cpp \
    ./src/SIMDAQThread.cpp

# root headers
ROOTDIR = /Users/benson/Desktop/root_x86_64
INCLUDEPATH += $$ROOTDIR/include

ROOTSYSLIB += $$ROOTDIR/lib
INCLUDEPATH += $$ROOTSYSLIB
LIBS += -L/$$ROOTSYSLIB -lCore -lHist

# Master environment variable
# ****YOU MUST SET THIS VARIABLE****
GRIFDIR=/Users/benson/Desktop/grif
GRIFPROJECTDIR=$$GRIFDIR/project/austin/FallTest

# run code generation
system(cd $$GRIFPROJECTDIR; ./setup.py)

QMAKE_CXXFLAGS += -D GRIF_CODE_GENERATION=1

