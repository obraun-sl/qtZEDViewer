#ifndef __STEREOLABS__AUTOCON__
#define __STEREOLABS__AUTOCON__

#include <QtCore>
#include <QtWidgets>
#include <QtGui>
#include "GUI.hpp"
#include "CaptureModule.hpp"

class MainProcess: public QObject {
    Q_OBJECT
public:
    MainProcess();
    ~MainProcess();
    void construct();

private:
    GUI * Ui = nullptr;
    QTimer* displayTimer= nullptr;
    CaptureModule* capture= nullptr;

private slots :
    void setContext();

};
#endif /*__STEREOLABS__AUTOCON__*/
