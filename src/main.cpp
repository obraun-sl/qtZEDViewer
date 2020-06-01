#include "MainProcess.hpp"
#include <QDesktopWidget>
#include <QDebug>

int main(int argc, char** argv) {
    QApplication a(argc,argv);
    MainProcess z_app;
    return a.exec();
}
