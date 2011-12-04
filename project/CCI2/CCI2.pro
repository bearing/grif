#############################################
# Compact Compton Imaging System #
#############################################

QT       += core network qt3support xml
QT       += gui

TARGET = CCI2
CONFIG   += console
TEMPLATE = app
FORMS += mainwindow.ui \
    source/runwindow.ui

include(../../../framework/trunk/source/fsource.pri)
include(../../../framework/trunk/include/finclude.pri)
include(../../../application/daq/sis3150usb2.0/sis3150.pri)
include(../../../application/gui/grihistwidget/trunk/grihistwidget.pri)

DEPENDPATH += source include ../../../application/analysis/sishdf5

INCLUDEPATH += include\
    source\
    ../../../project/cameron/CCI2/include \
    ../../../application/analysis/sishdf5

HEADERS += \
    ../../../application/analysis/sishdf5/HDF5Analysis.h \
    GRIUserProcesses.h \
    netanalysisthread.h \
    CCI2Run.h \
    mainwindow.h \
    runwindow.h \
    /usr/include/H5Cpp.h \
    source/ccidatasplitter.h \
    source/ccidatasorter.h \
    source/ccieventreconstruction.h


# Source file and headers
SOURCES += \
    main.cpp \
    CCI2Run.cpp \
    HDF5Analysis.cpp \
    GRIUserLoader.cpp \
    netanalysisthread.cpp \
    mainwindow.cpp \
    runwindow.cpp \
    source/ccidatasplitter.cpp \
    source/ccidatasorter.cpp \
    source/ccieventreconstruction.cpp

DEFINES += SISUSB_PATH=\\\"/opt/\\\"

# load in all ROOT libraries
unix:INCLUDEPATH += /usr/include/root #root headers
unix:ROOTSYSLIB += /usr/lib/root

win32:INCLUDEPATH += C:/root/include #root headers
win32:ROOTSYSLIB += C:/root/lib

INCLUDEPATH += $$ROOTSYSLIB
LIBS += -L$$ROOTSYSLIB
unix:LIBS += -L$$ROOTSYSLIB -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lRint -lPostscript
unix:LIBS += -L$$ROOTSYSLIB -lMatrix -lPhysics -lTree #-llibpthread -ldl -lrdynamic -lm -llibz
unix:LIBS += -L$$ROOTSYSLIB -lMathCore -lThread  -lGui -lGpad -lSpectrum

#windows root library names
win32:LIBS += -L$$ROOTSYSLIB -llibCore -llibCint -llibRIO -llibNet -llibHist -llibGraf -llibGraf3d -llibRint -llibPostscript
win32:LIBS += -L$$ROOTSYSLIB -llibMatrix -llibPhysics -llibTree #-llibpthread -ldl -lrdynamic -lm -llibz
win32:LIBS += -L$$ROOTSYSLIB -llibMathCore -llibThread  -llibGui -llibGpad -llibSpectrum

#HDF5 libraries
unix: LIBS += -L/usr/lib -lhdf5_cpp

win32: DEFINES += HDF5CPP_USEDLL
win32: LIBS += -LC:/HDF5/lib -lhdf5_cppdll -lhdf5dll







