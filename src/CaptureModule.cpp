#include "CaptureModule.hpp"
#include <QDebug>


CaptureModule::CaptureModule() {
    context = 0;
    closed_=false;
    opened_=false;
}

CaptureModule::~CaptureModule() {
    close();
}

void CaptureModule::close() {
    if (closed_) return;

    closed_=true;
    run_ = false;
    QThread::msleep(1);
    //Wait for thread to finish
    wait();

    //suppress memory allocated
    gpuImage.free();
    gpuDepth.free();
    zed_.close();
    cudaGetLastError(); //to catch error
}



bool CaptureModule::open() {

    sl::InitParameters init_parameters;
    init_parameters.coordinate_units = sl::UNIT::METER; // don't care
    init_parameters.coordinate_system = sl::COORDINATE_SYSTEM::RIGHT_HANDED_Y_UP; // OpenGL's coordinate system is right_handed
#ifdef __aarch64__
    init_parameters.depth_mode = sl::DEPTH_MODE::PERFORMANCE;
#else
    init_parameters.depth_mode = sl::DEPTH_MODE::ULTRA;
#endif
    init_parameters.sdk_verbose = true;

    opened_ = false;
    run_= false;
    sl::ERROR_CODE ret = this->zed_.open(init_parameters);
    if (ret==sl::ERROR_CODE::SUCCESS) {
        cudaGetLastError(); //to catch error
        context = zed_.getCUDAContext();
        cuCtxSetCurrent(context);
        sl::Resolution cam_res = zed_.getCameraInformation().camera_resolution;
        gpuImage.alloc(cam_res, sl::MAT_TYPE::U8_C4, sl::MEM::GPU);
        gpuDepth.alloc(cam_res, sl::MAT_TYPE::U8_C4, sl::MEM::GPU);
        if (cudaGetLastError() != 0)
            return false;
        else
        {
            opened_ = true;
            run_ = true;
            return true;
        }
    }
    return false;
}

void CaptureModule::startCapture()
{
    if (opened_ && run_)
        start();
}

void CaptureModule::run() {
    sl::RuntimeParameters grab_params;
    grab_params.enable_depth = true;
    while (run_) {
        cuCtxSetCurrent(context);
        sl::ERROR_CODE err_ = this->zed_.grab(grab_params);
        if (err_ == sl::ERROR_CODE::SUCCESS) {
            zed_.retrieveImage(gpuImage,sl::VIEW::LEFT,sl::MEM::GPU);
            zed_.retrieveImage(gpuDepth,sl::VIEW::DEPTH,sl::MEM::GPU);
            emit triggerImage();
        } else {
            if (err_ == sl::ERROR_CODE::CAMERA_NOT_DETECTED) {
                run_ = false;
            } else
                sl::sleep_ms(1);
        }
    }
}
