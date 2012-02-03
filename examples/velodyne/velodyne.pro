#############################################
# Velodyne Lidar #
#############################################

QT       += core xml
QT       += gui

TARGET =  velodyne
CONFIG   += console
TEMPLATE = app
FORMS +=

include(../../framework/source/fsource.pri)
include(../../framework/include/finclude.pri)

DEPENDPATH += source include . \
    velodynedaq \
    velodyneanalysis

INCLUDEPATH += include velodynedaq \
                velodyneanalysis

HEADERS += \
    /usr/include/H5Cpp.h \
    velodynedaq.h \
    velodynedata.h \
    velodyneanalysis.h

# Source file and headers
SOURCES += \
    main.cpp \
    velodynedaq.cpp \
    velodyneanalysis.cpp

QMAKE_CXXFLAGS += -D GRIF_CODE_GENERATION=1 -O2

DEFINES += SISUSB_PATH=\\\"/opt/\\\"

GRIFPROJECTDIR = /home/cameron/grif/examples/velodyne


#GRIFPROJECTDIR = $$GRIFDIR/examples/simulator
system(cd $$GRIFPROJECTDIR; python2 setup.py)

DEFINES += GRIFPROJECTDIR=$${GRIFPROJECTDIR}

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