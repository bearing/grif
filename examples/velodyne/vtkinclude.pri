# include all of the vtk header files
VTKINCLUDE = /usr/include/vtk-5.8
VTKLIB = /usr/lib/vtk-5.8
INCLUDEPATH += $$VTKINCLUDE

LIBS += -L$$VTKLIB \
    -lMapReduceMPI \
    -lQVTK \
    -lmpistubs \
    -lvtkCharts \
    -lvtkCommon \
    -lvtkDICOMParser \
    -lvtkFiltering \
    -lvtkGenericFiltering \
    -lvtkGeovis \
    -lvtkGraphics \
    -lvtkHybrid \
    -lvtkIO \
    -lvtkImaging \
    -lvtkInfovis \
    -lvtkNetCDF \
    -lvtkNetCDF_cxx \
   # -lvtkQtChart \
    -lvtkRendering \
    -lvtkViews \
    -lvtkVolumeRendering \
    -lvtkWidgets \
    -lvtkalglib \
    -lvtkexoIIc \
   # -lvtkexpat \
    #-lvtkfreetype \
    -lvtkftgl \
    #-lvtkjpeg \
    #-lvtklibxml2 \
    -lvtkmetaio \
    #-lvtkpng \
    -lvtkproj4 \
    -lvtksqlite \
    -lvtksys \
   # -lvtktiff \
    -lvtkverdict
   # -lvtkzlib
