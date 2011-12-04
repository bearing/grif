#############################################
# Compact Compton Imaging System #
#############################################

QT       += core network qt3support xml
QT       += gui

TARGET =  velodyne
CONFIG   += console
TEMPLATE = app
FORMS +=

include(../../../framework/trunk/source/fsource.pri)
include(../../../framework/trunk/include/finclude.pri)
include(vtkinclude.pri)
#include(../../../application/daq/sis3150usb2.0/sis3150.pri)
#include(../../../application/gui/grihistwidget/trunk/grihistwidget.pri)

DEPENDPATH += source include . \
    ../../../application/daq/velodynehdl32 \
    ../../../application/analysis/velodyne


INCLUDEPATH += include ../../../application/daq/velodynehdl32 \
                ../../../application/analysis/velodyne \
                /usr/include/pcl-1.2 \
                /usr/include/eigen3 \
                /usr/include/flann



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
win32:LIBS += -llibMathCore -llibThread  -llibGui -llibGpad -llibSpectrum

#PCL
unix:LIBS += -lpcl_common -lpcl_io -lpcl_visualization -lpcl_features -lpcl_filters -lpcl_kdtree -lpcl_keypoints -lpcl_octree\
    -lpcl_range_image -lpcl_range_image_border_extractor -lpcl_registration -lpcl_sample_consensus -lpcl_segmentation -lpcl_surface


#HDF5 libraries
unix: LIBS += -L/usr/lib -lhdf5_cpp

unix: LIBS += -lpcap

win32: DEFINES += HDF5CPP_USEDLL
win32: LIBS += -LC:/HDF5/lib -lhdf5_cppdll -lhdf5dll





