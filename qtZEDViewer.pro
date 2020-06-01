#-------------------------------------------------
#
# Project created by QtCreator 2014-10-22T13:51:39
#
#-------------------------------------------------

TARGET = "qtZedViewer"
TEMPLATE = app
DESTDIR = build
CONFIG += console
QT += gui core widgets opengl


QMAKE_CFLAGS += -std=c++0x -O3 -g -fpermissive -fPIC
QMAKE_CXXFLAGS += -std=c++0x -w  -g -O3 -fpermissive -fPIC
QMAKE_LFLAGS += -std=c++0x -O3 -g -fpermissive -fPIC
CONFIG += silent

## include all platform
INCLUDEPATH +=include

## Flags is using Debug libs
#CONFIG(debug,debug|release)
#{
#    DEFINES += _GLIBCXX_DEBUG
#}


## INC directories for Windows
win32:INCLUDEPATH  += "$$(ZED_SDK_ROOT_DIR)/include"
## LINK directories for Windows
win32:LIBS += -L"$$(ZED_SDK_ROOT_DIR)/lib"

## INC directories for Unix
unix:INCLUDEPATH  += "/usr/include/GL"
unix:INCLUDEPATH  += "/usr/local/zed/include"
unix:INCLUDEPATH  += "/usr/local/include"

## LINK directories for Unix
unix:LIBS += -L"/usr/local/lib"
unix:LIBS += -L"/usr/local/zed/lib"
unix:LIBS += -L"/usr/lib/aarch64-linux-gnu/tegra/"

################### SPECIFIC CUDA VERSION
CUDA_NB = 102 ## Set CUDA version here

equals(CUDA_NB,90){
CUDA_VERSION="9.0"
win32:CUDA_PATH = $$(CUDA_PATH_V9_0)
win32:INCLUDEPATH += "$$(CUDA_PATH_V9_0)/include"
win32:LIBS += -L"$$(CUDA_PATH_V9_0)/lib/x64"
unix:INCLUDEPATH  += "/usr/local/cuda-9.0/include"
unix:LIBS += -L"/usr/local/cuda/lib64"
unix:QMAKE_LFLAGS = -Wl,-rpath,/usr/local/cuda/lib64
}

equals(CUDA_NB,91){
CUDA_VERSION="9.1"
win32:CUDA_PATH = $$(CUDA_PATH_V9_1)
win32:INCLUDEPATH += "$$(CUDA_PATH_V9_1)/include"
win32:LIBS += -L"$$(CUDA_PATH_V9_1)/lib/x64"
unix:INCLUDEPATH  += "/usr/local/cuda-9.1/include"
unix:LIBS += -L"/usr/local/cuda/lib64"
unix:QMAKE_LFLAGS = -Wl,-rpath,/usr/local/cuda/lib64
}

equals(CUDA_NB,92){
CUDA_VERSION="9.2"
win32:CUDA_PATH = $$(CUDA_PATH_V9_2)
win32:INCLUDEPATH += "$$(CUDA_PATH_V9_2)/include"
win32:LIBS += -L"$$(CUDA_PATH_V9_2)/lib/x64"
unix:INCLUDEPATH  += "/usr/local/cuda-9.2/include"
unix:LIBS += -L"/usr/local/cuda/lib64"
unix:QMAKE_LFLAGS = -Wl,-rpath,/usr/local/cuda/lib64
}

equals(CUDA_NB,100){
CUDA_VERSION="10.0"
win32:CUDA_PATH = $$(CUDA_PATH_V10_0)
win32:INCLUDEPATH += "$$(CUDA_PATH_V10_0)/include"
win32:LIBS += -L"$$(CUDA_PATH_V10_0)/lib/x64"
unix:INCLUDEPATH  += "/usr/local/cuda-10.0/include"
unix:LIBS += -L"/usr/local/cuda/lib64"
unix:QMAKE_LFLAGS = -Wl,-rpath,/usr/local/cuda/lib64
}

equals(CUDA_NB,102){
CUDA_VERSION="10.2"
win32:CUDA_PATH = $$(CUDA_PATH_V10_2)
win32:INCLUDEPATH += "$$(CUDA_PATH_V10_2)/include"
win32:LIBS += -L"$$(CUDA_PATH_V10_2)/lib/x64"
unix:INCLUDEPATH  += "/usr/local/cuda-10.2/include"
unix:LIBS += -L"/usr/local/cuda/lib64"
unix:QMAKE_LFLAGS = -Wl,-rpath,/usr/local/cuda/lib64
}


################################
## TARGET LIBS win
win32:LIBS += -lsl_zed64
win32:LIBS += -lcuda
win32:LIBS += -lcudart
win32:LIBS += -lnppicom
win32:LIBS += -lnppial
win32:LIBS += -lnppidei
win32:LIBS += -lnppif
win32:LIBS += -lnppig
win32:LIBS += -lnppim
win32:LIBS += -lnppist
win32:LIBS += -lnppisu
win32:LIBS += -lnppitc
win32:LIBS += -lnppc
win32:LIBS += -lnpps
win32:LIBS += -lOpenGL32


## TARGET LIBS unix
unix:LIBS += -lsl_zed
unix:LIBS += -lGL
unix:LIBS += -lcuda
unix:LIBS += -lcudart
unix:LIBS += -lnppicom
unix:LIBS += -lnppial
unix:LIBS += -lnppidei
unix:LIBS += -lnppif
unix:LIBS += -lnppig
unix:LIBS += -lnppim
unix:LIBS += -lnppist
unix:LIBS += -lnppisu
unix:LIBS += -lnppitc
unix:LIBS += -lnppc
unix:LIBS += -lnpps


######################################################"

##source of application
SOURCES += \
    src/main.cpp \
    src/OGLWindow.cpp \
    src/MainProcess.cpp \
    src/CaptureModule.cpp \
    src/GUI.cpp \

HEADERS += \
    include/OGLWindow.hpp \
    include/MainProcess.hpp \
    include/CaptureModule.hpp \
    include/GUI.hpp \

