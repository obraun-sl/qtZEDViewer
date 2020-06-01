#ifndef __Qt_OGLWindow_H__
#define __Qt_OGLWindow_H__

#include <QGLWidget>
#include <QGLFramebufferObject>
#include <QtOpenGL>
#include <QtGui>
#include <QtWidgets>

#include "cuda.h"
#include "cuda_runtime.h"
#include "cuda_gl_interop.h"

#include <sl/Camera.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

class OGLWindow : public QOpenGLWidget {

    Q_OBJECT

public:
    OGLWindow(QWidget* Parent = nullptr);
    ~OGLWindow();

    void Deinit();
    void InitGLProcess(CUcontext &ctx, sl::Mat &im);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    bool DisplayImage;
    bool can_draw;
    bool data_;

    unsigned char* p_d_data;
    int im_w, im_h, im_s;

    int x_viewport;
    int y_viewport;
    int w_viewport;
    int h_viewport;

    CUcontext context;
    QGLShaderProgram* program;
    GLuint imageTex_;
    cudaGraphicsResource* pcuImageRes_;
    void createShader();

public slots:
    void Ingest();

signals:
    void init(bool);
};

#endif /* __OGL_2D_WIDGET_H__ */
