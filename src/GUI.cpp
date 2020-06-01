#include "GUI.hpp"

GUI::GUI(QWidget* parent) {

    imageGLViewer = nullptr;
    depthGLViewer = nullptr;
}

GUI::~GUI() {

}

void GUI::construct() {
   imageGLViewer = new OGLWindow(this);
   depthGLViewer = new OGLWindow(this);
}

void GUI::destruct()
{

    if (imageGLViewer) {
        delete imageGLViewer;
        imageGLViewer = nullptr;
    }

    if (depthGLViewer) {
        delete depthGLViewer;
        depthGLViewer = nullptr;
    }

}

void GUI::updateImage()
{
    imageGLViewer->update();
    depthGLViewer->update();
}


void GUI::resizeEvent(QResizeEvent* ev)
{
    int appw = ev->size().width();
    int apph = ev->size().height();

    if (imageGLViewer) imageGLViewer->setGeometry(0,0,appw/2,apph);
    if (depthGLViewer) depthGLViewer->setGeometry(appw/2,0,appw/2,apph);
}
