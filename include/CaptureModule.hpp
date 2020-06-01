
#ifndef __CAP_MODULE_H__
#define __CAP_MODULE_H__

#include <QtCore/qthread.h>
#include <sl/Camera.hpp>

class CaptureModule: public QThread {
    Q_OBJECT

public:
    // Constructeur
    CaptureModule();
    // Desctructeur
    ~CaptureModule();

    void close();
    bool open();
    void startCapture();

    sl::Mat gpuImage;
    sl::Mat gpuDepth;
    CUcontext context;
    sl::Camera zed_;

protected:
    void run();

private:
    bool closed_=false;
    bool opened_=false;
    bool run_ = false;



signals:
    void triggerImage();
};

#endif /* __InputThreadModul_H__ */
