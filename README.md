# qtZedViewer

* ZED Camera Left+Depth display using Qt (QtGui,QtOpenGL,...) with qmake generator
* It shows how to use GPU and QtOpenGL interoperability (based on CUDA-OpenGL interop)


### Prerequisites

- Windows 10, Ubuntu LTS
- [ZED SDK](https://www.stereolabs.com/developers/) and its dependencies ([CUDA](https://developer.nvidia.com/cuda-downloads))

### Pre-build
Set your CUDA version used in the pro file : 
https://github.com/obraun-sl/qtZEDViewer/blob/master/qtZEDViewer.pro#L45

For example, if you are using CUDA 10.2, set 
CUDA_NB = 102
If you are using CUDA 10.0, set 
CUDA_NB = 100


### Build
In the folder where the .pro file is located :
$ qmake && make

This will generate the executable inside the build directory.


### Compatibility
Compatible with Qt5 and ZED SDK 3.X
