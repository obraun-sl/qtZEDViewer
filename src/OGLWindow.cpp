#include "OGLWindow.hpp"

#include <ctime>
#include <chrono>
#include <QElapsedTimer>


OGLWindow::OGLWindow(QWidget* parent) : QOpenGLWidget(parent) {
    this->format().setSamples(4);

    program=nullptr;
    data_ = false;
    DisplayImage = false;
    can_draw = false;
    p_d_data = nullptr;

    x_viewport = 0;
    y_viewport = 0;
    w_viewport = parent ? parent->width() : 1920 / 2;
    h_viewport = parent ? parent->height() : 1080 / 2;
}

void OGLWindow::InitGLProcess(CUcontext& ctx, sl::Mat& im, bool disp_image) {
    context = ctx;
    this->im_w = im.getWidth();
    this->im_h = im.getHeight();
    this->im_s = im.getStepBytes(sl::MEM::GPU);
    p_d_data = im.getPtr<unsigned char>(sl::MEM::GPU);
    DisplayImage = disp_image;
    
    makeCurrent();
    cuCtxSetCurrent(context);
    cudaError err1;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &this->imageTex_);
    glBindTexture(GL_TEXTURE_2D, this->imageTex_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->im_w, this->im_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    unsigned char *tmpS = (unsigned char*)malloc(this->im_w* this->im_h*4);
    memset(tmpS,0,this->im_w* this->im_h*4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->im_w, this->im_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tmpS);
    free(tmpS);
    glBindTexture(GL_TEXTURE_2D, 0);

    err1 = cudaGraphicsGLRegisterImage(&this->pcuImageRes_, this->imageTex_, GL_TEXTURE_2D, cudaGraphicsMapFlagsNone);

    if (err1 != cudaSuccess) {
        std::cout <<"[ZED Depth Viewer] "<<"Cuda Error: "<<cudaGetErrorName(err1) << std::endl;
        return;
    }

    resizeGL(this->width(), this->height());
    can_draw = true;
    emit init(true);
}

void OGLWindow::createShader()
{
    #define PROGRAM_VERTEX_ATTRIBUTE 0
    #define PROGRAM_TEXCOORD_ATTRIBUTE 1

    QGLShader *vshader = new QGLShader(QGLShader::Vertex, this);
    const char *vsrc =
        "#version 330 core\n"
        "attribute highp vec4 vertex;\n"
        "attribute mediump vec4 texCoord;\n"
        "varying mediump vec4 texc;\n"
        "uniform mediump mat4 matrix;\n"
        "void main(void)\n"
        "{\n"
        "    gl_Position = matrix * vertex;\n"
        "    texc = texCoord;\n"
        "}\n";
    vshader->compileSourceCode(vsrc);


    QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);
    const char *fsrc=
        "#version 330 core\n"
        "uniform sampler2D texture;\n"
        "varying mediump vec4 texc;\n"
        "void main(void)\n"
        "{\n"
        "    vec4 colin =texture2D(texture, texc.st);\n "
        "    vec4 colout =vec4(colin.b,colin.g,colin.r,colin.a);\n "
        "    gl_FragColor =colout;\n"
        "}\n";

    fshader->compileSourceCode(fsrc);

    program = new QGLShaderProgram(this);
    program->addShader(vshader);
    program->addShader(fshader);
    program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    program->link();
    program->bind();
    program->setUniformValue("texture", 0);
    program->release();
}

void OGLWindow::Deinit() {
    p_d_data = nullptr;
    if (can_draw) {
        makeCurrent();
        glClearColor(0,0,0, 1.);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    glDeleteTextures(1, &this->imageTex_);
    can_draw = false;
}

OGLWindow::~OGLWindow() {
    DisplayImage = false;
    data_ = false;
    can_draw = false;
    Deinit();
       if (program)
       {
           delete program;
           program = nullptr;
       }


}

void OGLWindow::initializeGL() {
    makeCurrent();
    createShader();
}


void OGLWindow::Ingest()
{
    if (p_d_data) {
        makeCurrent();
        cuCtxSetCurrent(context);
        cudaArray_t ArrIm;
        cudaGraphicsMapResources(1, &this->pcuImageRes_);
        cudaGraphicsSubResourceGetMappedArray(&ArrIm, this->pcuImageRes_, 0, 0);
        cudaMemcpy2DToArray(ArrIm, 0, 0, (unsigned char*) p_d_data, this->im_s, this->im_w * 4 * sizeof (uchar), this->im_h, cudaMemcpyDeviceToDevice);
        cudaGraphicsUnmapResources(1, &this->pcuImageRes_, 0);
        cudaGetLastError();
    }
}

void OGLWindow::paintGL() {

    makeCurrent();

    if (can_draw && p_d_data) {
        glClearColor(0.0,0.0,0.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cuCtxSetCurrent(context);

        QVector<QVector3D> vertices;
        QVector<QVector2D> texCoords;
        texCoords.append(QVector2D(1,0));
        texCoords.append(QVector2D(0,0));
        texCoords.append(QVector2D(0,1));
        texCoords.append(QVector2D(1,1));
        vertices.append(QVector3D(1, -1,0));
        vertices.append(QVector3D(-1, -1,0));
        vertices.append(QVector3D(-1, 1,0));
        vertices.append(QVector3D(1, 1,0));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(x_viewport, y_viewport, w_viewport, h_viewport);

        program->bind();
        QMatrix4x4 m_proj;
        m_proj.setToIdentity();
        m_proj.ortho(QRectF(-1,-1,2,2.0));

        program->setUniformValue("matrix", m_proj);
        program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
        program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
        program->setAttributeArray(PROGRAM_VERTEX_ATTRIBUTE,vertices.constData());
        program->setAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE, texCoords.constData());
        glBindTexture(GL_TEXTURE_2D,imageTex_);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindTexture(GL_TEXTURE_2D,0);
        program->release();
    } else {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void OGLWindow::resizeGL(int width, int height) {
    int c_w = width;
    int c_h = height;

    float ratio_ = im_w / (1.f * im_h);

    int h_init = c_h;
    int w_init = c_w;
    int x_init = 0;
    int y_init = 0;

    if (ratio_ < (c_w / (1.f * c_h))) {
        w_init = c_h*ratio_;
        x_init = abs(c_w - w_init) / 2;
    } else {
        h_init = w_init / ratio_;
        y_init = abs(c_h - h_init) / 2;
    }

    x_viewport = x_init;
    y_viewport = y_init;
    w_viewport = w_init;
    h_viewport = h_init;
    glViewport(x_viewport, y_viewport, w_viewport, h_viewport);
}

