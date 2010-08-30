# -------------------------------------------------
# Project created by QtCreator 2010-07-21T18:07:37
# -------------------------------------------------
TARGET = GammaRayImaging
QT += xml
QT += network
TEMPLATE = app
CONFIG += console
SOURCES += source/GRIXMLParser.cpp \
    source/GRIServerThread.cpp \
    source/GRIServer.cpp \
    source/GRIRunManager.cpp \
    source/GRIRegulator.cpp \
    source/GRIProcessThread.cpp \
    source/GRIProcessObj.cpp \
    source/GRIMemoryManager.cpp \
    source/GRILoader.cpp \
    source/GRIDataBlock.cpp \
    source/GRICLI.cpp \
    source/GRICommandAndControl.cpp \
    source/GRIClientSocket.cpp \
    source/GRIBuffer.cpp \
    source/main.cpp
HEADERS += include/GRIXMLParser.h \
    include/GRIServerThread.h \
    include/GRIServer.h \
    include/GRIRunManager.h \
    include/GRIRegulator.h \
    include/GRIProcessThread.h \
    include/GRIParamList.h \
    include/GRIMemoryManager.h \
    include/GRILoader.h \
    include/GRIDataBlock.h \
    include/GRICLI.h \
    include/GRICommandAndControl.h \
    include/GRIClientSocket.h \
    include/GRIBuffer.h \
    include/GRIProcessObj.h
INCLUDEPATH += include
OTHER_FILES += lib/file_paths.xml \
    lib/struc_config.xml \
    lib/runtime_params.xml
RESOURCES += lib/GRI_resource.qrc
