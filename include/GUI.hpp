#ifndef __SCANNER_GUI_H__
#define __SCANNER_GUI_H__
#include <QtWidgets/QWidget>
#include <QEvent>
#include <QMimeData>
#include <QtWidgets/QMessageBox>
#include <QPainter>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QMainWindow>
#include "OGLWindow.hpp"


class GUI: public QMainWindow {
    Q_OBJECT
public:
    GUI(QWidget* parent = nullptr);
    ~GUI();

    void construct();
    void destruct();

    OGLWindow* getImageViewer() {return imageGLViewer;}
    OGLWindow* getDepthViewer() {return depthGLViewer;}


private :
    OGLWindow* imageGLViewer;
    OGLWindow* depthGLViewer;

public slots :
    void updateImage();

protected :
    virtual void resizeEvent(QResizeEvent* ev);
};

#endif /* __SCANNER_GUI_H__ */
