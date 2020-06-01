# qtZedViewer

* ZED Camera Left+Depth display using Qt (QtGui,QtOpenGL,...) with qmake generator
* It shows how to use GPU and QtOpenGL intereoperability (based on CUDA-OpenGL interop)


### Prerequisites

- Windows 10, Ubuntu LTS
- [ZED SDK](https://www.stereolabs.com/developers/) and its dependencies ([CUDA](https://developer.nvidia.com/cuda-downloads))


### Build
In the folder where the .pro file is located :
$ qmake && make

This will generate the executable inside the build directory.


### Compatibility
Compatible with Qt5 and ZED SDK 3.X
