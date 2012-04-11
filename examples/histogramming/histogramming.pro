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

QT       += core network qt3support xml
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


QMAKE_CXXFLAGS += -D GRIF_CODE_GENERATION=1 -O3

# Top directory of GRIF on your machine
GRIFDIR=/Users/markbandstra/Projects/GRIF/grif

# run code generation
GRIFPROJECTDIR = $$GRIFDIR/examples/histogramming
system(cd $$GRIFPROJECTDIR)
system(python setup.py)

QMAKE_CXXFLAGS += -D GRIF_CODE_GENERATION=1

DEFINES += GRIFPROJECTDIR=$${GRIFPROJECTDIR}

# External libraries
INCLUDEPATH += $$GRIFDIR/external

# ROOT headers
ROOTDIR = /Users/markbandstra/Software/root
INCLUDEPATH += $$ROOTDIR/include

# ROOT libraries
ROOTSYSLIB += $$ROOTDIR/lib
INCLUDEPATH += $$ROOTSYSLIB
LIBS += $$ROOTSYSLIB/libCint.so
LIBS += $$ROOTSYSLIB/libMatrix.so
LIBS += $$ROOTSYSLIB/libMathCore.so
LIBS += -L$$ROOTSYSLIB
LIBS += -L$$ROOTSYS/lib -lCore -lHist
