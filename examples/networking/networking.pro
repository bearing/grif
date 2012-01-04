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
QT       -= gui

# Change this variable to whatever your project name is
TARGET = networking

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
    ./include/TcpAnalysisThread.h \
    ./include/TcpDAQThread.h
SOURCES += \
    ./src/main.cpp \
    ./src/TcpAnalysisThread.cpp \
    ./src/TcpDAQThread.cpp

# GRIFDIR is the head directory of GRIF on your machine
# ***You must set GRIFDIR as an environment variable***
GRIFDIR=$$(PWD)/../..

ROOTDIR=/Users/benson/Desktop/root_x86_64

# root headers
INCLUDEPATH += $$ROOTDIR/include

ROOTSYSLIB += $$ROOTDIR/lib
INCLUDEPATH += $$ROOTSYSLIB
LIBS += $$ROOTSYSLIB/libCint.so
LIBS += $$ROOTSYSLIB/libMatrix.so
LIBS += $$ROOTSYSLIB/libMathCore.so
LIBS += -L/$$ROOTSYSLIB
LIBS += -L$$ROOTSYS/lib -lCore -lHist

EXTDIR = $$GRIFDIR/external
INCLUDEPATH += EXTDIR

# run code generation
macx|unix {
    system(cd $$GRIFPROJECTDIR; ./setup.py)
}
win32 {
    system(cd $$GRIFPROJECTDIR; python setup.py)
}

QMAKE_CXXFLAGS += -D GRIF_CODE_GENERATION=1 -O3
