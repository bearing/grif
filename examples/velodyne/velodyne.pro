#############################################
# Velodyne Lidar #
#############################################

QT       += core network xml
QT       += gui

TARGET =  velodyne
CONFIG   += console
TEMPLATE = app
FORMS +=

include(../../framework/source/fsource.pri)
include(../../framework/include/finclude.pri)
include(vtkinclude.pri)
#include(../../../application/daq/sis3150usb2.0/sis3150.pri)
#include(../../../application/gui/grihistwidget/trunk/grihistwidget.pri)

DEPENDPATH += source include . \
    velodynedaq \
    velodyneanalysis


INCLUDEPATH += include velodynedaq \
                velodyneanalysis

HEADERS += \
    /usr/include/H5Cpp.h \
    GRIUserProcesses.h \
    velodynedaq.h \
    velodynedata.h \
    velodyneanalysis.h \
    HDF5Analysis.h


# Source file and headers
SOURCES += \
    main.cpp \
    GRIUserLoader.cpp \
    velodynedaq.cpp \
    velodyneanalysis.cpp \
    HDF5Analysis.cpp

DEFINES += SISUSB_PATH=\\\"/opt/\\\"

# load in all ROOT libraries
unix:INCLUDEPATH += /usr/include/root #root headers
unix:ROOTSYSLIB += /usr/lib/root

win32:INCLUDEPATH += C:/root/include #root headers
win32:ROOTSYSLIB += C:/root/lib

INCLUDEPATH += $$ROOTSYSLIB
LIBS += -L$$ROOTSYSLIB
unix:LIBS += -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lRint -lPostscript
unix:LIBS += -lMatrix -lPhysics -lTree #-llibpthread -ldl -lrdynamic -lm -llibz
unix:LIBS += -lMathCore -lThread  -lGui -lGpad -lSpectrum

#windows root library names
win32:LIBS += -llibCore -llibCint -llibRIO -llibNet -llibHist -llibGraf -llibGraf3d -llibRint -llibPostscript
win32:LIBS += -llibMatrix -llibPhysics -llibTree #-llibpthread -ldl -lrdynamic -lm -llibz
win32:LIBS += -llibMathCore -llibThread  -llibGui -llibGpad -llibSpectrumssss

#HDF5 libraries
unix: LIBS += -L/usr/lib -lhdf5_cpp -lhdf5

unix: LIBS += -lpcap

win32: DEFINES += HDF5CPP_USEDLL
win32: LIBS += -LC:/HDF5/lib -lhdf5_cppdll -lhdf5dll





