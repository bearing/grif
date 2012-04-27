# Copyright (C) 2012 Gamma-ray Imaging Framework Team
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 3.0 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
# 
# The license can be found in the LICENSE.txt file.
# 
# Contact:
# Dr. Daniel Chivers
# dhchivers@lbl.gov

QT       += core network xml
QT       += gui

TARGET = GRIHistWidgetTest
CONFIG   += console
TEMPLATE = app

DEPENDPATH += ../../ . \
        ./src \
        ./include \
        ../../framework/source \
        ../../framework/include \
        ../../framework/source/hist \
        ../../framework/include/hist


INCLUDEPATH += ../../ . \
        ./include \
        ../../framework/include \
        ../../framework/source \
        ../../framework/source/hist \
        ../../framework/include/hist

include(../../framework/source/fsource.pri)
include(../../framework/include/finclude.pri)
include(../../framework/source/hist/histsource.pri)
include(../../framework/include/hist/histinclude.pri)

# Source file and headers
SOURCES += \
    ./src/main.cpp \
    ./src/SIMMCAnalysisThread.cpp \
    ./src/SIMDAQThread.cpp

HEADERS += \
    ./include/SIMMCAnalysisThread.h \
    ./include/SIMDAQThread.h

## 
# Please fill in GRIFDIR and ROOTDIR with the appropriate paths
# Top directory of GRIF on your machine:
GRIFDIR =
# Directory of ROOT on your machine:
ROOTDIR =



# run code generation
GRIFPROJECTDIR = $$GRIFDIR/examples/histogramming
system(cd $$GRIFPROJECTDIR)
system(python setup.py)

QMAKE_CXXFLAGS += -D GRIF_CODE_GENERATION=1 -O3

DEFINES += GRIFPROJECTDIR=$${GRIFPROJECTDIR}
# set up log directory
GRIF_LOG_DIR = $$GRIFPROJECTDIR/log/
win32 {
    GRIF_LOG_DIR = $$GRIFPROJECTDIR\\log
}


DEFINES += GRIF_LOG_DIR=\\\"$${GRIF_LOG_DIR}\\\"
# External libraries
INCLUDEPATH += $$GRIFDIR/external


# ROOT headers
INCLUDEPATH += $$ROOTDIR/include
# ROOT libraries
ROOTSYSLIB += $$ROOTDIR/lib
INCLUDEPATH += $$ROOTSYSLIB
LIBS += -L$$ROOTSYSLIB
LIBS += -L$$ROOTSYS/lib -lCore -lHist -lMatrix -lMathCore
LIBS += $$ROOTSYSLIB/libCint.so
LIBS += $$ROOTSYSLIB/libCore.so
LIBS += $$ROOTSYSLIB/libHist.so
LIBS += $$ROOTSYSLIB/libMatrix.so
LIBS += $$ROOTSYSLIB/libMathCore.so
