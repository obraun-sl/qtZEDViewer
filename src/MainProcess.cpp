#include "MainProcess.hpp"


MainProcess::MainProcess() {

    Ui = nullptr;
    displayTimer = nullptr;
    capture = nullptr;

#ifndef __aarch64__
    QSurfaceFormat fmt;
    fmt.setSamples(4);
    fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    QSurfaceFormat::setDefaultFormat(fmt);
#endif

    // Create GUI and center it
    Ui = new GUI();
    Ui->construct();
    QDesktopWidget desk;
    QRect rec = desk.availableGeometry(-1);
    Ui->setGeometry(rec.x() + rec.width()*0.05, rec.y() + rec.height()*0.05, rec.width()*0.9, rec.width()*0.9* 9.0/32.0);
    Ui->show();


    // Create Camera capture
    capture = new CaptureModule();
    if (capture->open())
    {
        setContext();
        capture->startCapture();
        connect(capture, SIGNAL(triggerImage()),Ui->getImageViewer(),SLOT(Ingest()));
        connect(capture, SIGNAL(triggerImage()),Ui->getDepthViewer(),SLOT(Ingest()));


        // Create display Time
        displayTimer = new QTimer(this);
        connect(displayTimer, SIGNAL(timeout()),Ui, SLOT(updateImage()));
        displayTimer->start(5);

    }


}

MainProcess::~MainProcess() {

    if (displayTimer) {
        displayTimer->stop();
        delete displayTimer;
        displayTimer = nullptr;
    }

    if (capture) {
        delete capture;
        capture = nullptr;
    }

    if (Ui) {
        delete Ui;
        Ui = nullptr;
    }
}

void MainProcess::setContext() {

    if (Ui->getImageViewer()!=nullptr)
    Ui->getImageViewer()->InitGLProcess(capture->context,capture->gpuImage);


    if (Ui->getDepthViewer()!=nullptr)
    Ui->getDepthViewer()->InitGLProcess(capture->context,capture->gpuDepth);

}

