######################################################################
# Automatically generated by qmake (1.07a) Mon Oct 17 01:38:18 2011
######################################################################

QT       += core network qt3support xml
CONFIG += qtestlib
TEMPLATE = app
CONFIG += moc
DEPENDPATH += .
DEPENDPATH +=    ../trunk/include
DEPENDPATH +=    ../trunk/source

INCLUDEPATH += .
INCLUDEPATH +=   ../trunk/include
INCLUDEPATH +=   ../trunk/source


#    ../../..trunk/include \
#   ../../trunk/include \
#    ../../../trunk/include \
#    ../../trunk/include \

#root headers
ROOTDIR = /home/ben/Downloads/root
INCLUDEPATH += $$ROOTDIR/include

ROOTSYSLIB += $$ROOTDIR/lib
INCLUDEPATH += $$ROOTSYSLIB
LIBS += $$ROOTSYSLIB/libCint.so
LIBS += $$ROOTSYSLIB/libMatrix.so
LIBS += $$ROOTSYSLIB/libMathCore.so
LIBS += -L/$$ROOTSYSLIB
LIBS += -L$$ROOTSYS/lib -lCore -lHist
# Input
SOURCES += GRIProcList_Test.cpp

