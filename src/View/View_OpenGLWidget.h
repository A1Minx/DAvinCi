#ifndef VIEW_OPENGLWIDGET_H
#define VIEW_OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QVector3D>
#include <Model.h>

class Controller;

class View_OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit View_OpenGLWidget(Model *model, Controller *controller, QWidget *parent)
    : QOpenGLWidget(parent), model(model), controller(controller),
      zoomLevel(1.0f), 
      pointVBO(0), tempPointVBO(0), lineVBO(0), tempLineVBO(0), gridVBO(0),
      gridSize(50.0f)
    {
        setFocusPolicy(Qt::StrongFocus);
    }

    ~View_OpenGLWidget() {
        makeCurrent();
        glDeleteBuffers(1, &pointVBO);
        glDeleteBuffers(1, &tempPointVBO);
        glDeleteBuffers(1, &lineVBO);
        glDeleteBuffers(1, &tempLineVBO);
        glDeleteBuffers(1, &gridVBO);
        doneCurrent();
    };

    inline virtual void update() {
        QOpenGLWidget::update();
    }; 

    virtual QVector3D screenToWorld(int x, int y) = 0;

protected:

    // ----- Architectural Variables -----
    Model *model;
    Controller *controller;     

    // ----- QT GL Methods -----
    inline virtual void initializeGL() {
        initializeOpenGLFunctions();
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // Dark Grey Background
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POINT_SMOOTH);
        glLineWidth(1.0f);

        setupBuffers();
    }

    inline virtual void resizeGL(int w, int h) {
        glViewport(0, 0, w, h); // TODO: Implement possibility to have multiple viewports in subclasses
        updateProjectionMatrix();
    }

    inline virtual void paintGL() {
        glClear(GL_COLOR_BUFFER_BIT);
    
        
        
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(projectionMatrix.constData());

        updateBuffers();
        drawGrid();
        drawPoints();  
        drawLines();    
        drawTempPoints();       
        drawTempLines(); 
    }

    // ----- Events -----
    virtual inline void wheelEvent(QWheelEvent *event) {
        float delta = event->angleDelta().y() / 120.0f;
        zoomLevel *= (1.0f + delta * 0.1f);
        zoomLevel = std::max(0.1f, std::min(10.0f, zoomLevel));
        
        updateProjectionMatrix();
        update();
    } 
    // TODO: Move logic to controller? Think about if this should ever be view independent and managed by modes.
    // TODO: If it stays in view only, keep this implementation.

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

    float zoomLevel;   

    // ----- Buffers -----
    inline virtual void setupBuffers() {
        // Initialize VBO Objects
        glGenBuffers(1, &pointVBO);
        glGenBuffers(1, &tempPointVBO);
        glGenBuffers(1, &lineVBO);
        glGenBuffers(1, &tempLineVBO);
        glGenBuffers(1, &gridVBO);
    }

    inline virtual void updateBuffers() {
        //TODO: Check if for loops can be optimized for performance, think about adding temp elements to the VBOs for persistent elements

        // Update points
        std::vector<float> newPointData;
        for (const std::shared_ptr<Point>& point : model->getPoints()) {
            newPointData.push_back(point->getX());
            newPointData.push_back(point->getY());
            newPointData.push_back(point->getZ());
        }
        if (newPointData != pointData) {
            pointData = newPointData;
            glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
            glBufferData(GL_ARRAY_BUFFER, pointData.size() * sizeof(float), pointData.data(), GL_DYNAMIC_DRAW);
        }

        // Update temporary points
        std::vector<float> newTempPointData;
        for (const TempPoint& point : model->getTempPoints()) {
            newTempPointData.push_back(point.x);
            newTempPointData.push_back(point.y);
            newTempPointData.push_back(point.z);
        }
        if (newTempPointData != tempPointData) {
            tempPointData = newTempPointData;
            glBindBuffer(GL_ARRAY_BUFFER, tempPointVBO);
            glBufferData(GL_ARRAY_BUFFER, tempPointData.size() * sizeof(float), tempPointData.data(), GL_DYNAMIC_DRAW);
        }

        // Update lines
        std::vector<float> newLineData;
        for (const std::shared_ptr<Line>& line : model->getLines()) {
            newLineData.push_back(line->getP1()->getX());
            newLineData.push_back(line->getP1()->getY());
            newLineData.push_back(line->getP1()->getZ());
            newLineData.push_back(line->getP2()->getX());
            newLineData.push_back(line->getP2()->getY());
            newLineData.push_back(line->getP2()->getZ());
        }
        if (newLineData != lineData) {
            lineData = newLineData;
            glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
            glBufferData(GL_ARRAY_BUFFER, lineData.size() * sizeof(float), lineData.data(), GL_DYNAMIC_DRAW);
        }

        // Update temp lines
        std::vector<float> newTempLineData;
        for (const TempLine& line : model->getTempLines()) {
            newTempLineData.push_back(line.x1);
            newTempLineData.push_back(line.y1);
            newTempLineData.push_back(line.z1);
            newTempLineData.push_back(line.x2);
            newTempLineData.push_back(line.y2);
            newTempLineData.push_back(line.z2);
        }
        if (newTempLineData != tempLineData) {
            tempLineData = newTempLineData;
            glBindBuffer(GL_ARRAY_BUFFER, tempLineVBO);
            glBufferData(GL_ARRAY_BUFFER, tempLineData.size() * sizeof(float), tempLineData.data(), GL_DYNAMIC_DRAW);
        }

        //TODO: Add Grid to VBO, check if it needs to be implemented in subclasses or if a common implementation can be found
    }

    GLuint pointVBO;
    GLuint tempPointVBO;
    GLuint lineVBO;
    GLuint tempLineVBO;
    GLuint gridVBO;

    // ----- Drawing -----
    virtual void drawGrid() = 0; 
    // Grid needs different implementation for every view. Make sure grid is geometrically aligned to World instead of screen.

    inline virtual void drawPoints() {
        if (pointData.empty()) return;

        glColor3f(1.0f, 0.0f, 0.0f);  // Red
        glPointSize(5.0f);
        
        glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, 0);  
        glDrawArrays(GL_POINTS, 0, pointData.size() / 3);
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    inline virtual void drawLines() {
        if (lineData.empty()) return;

        glColor3f(1.0f, 0.0f, 0.0f);  // Red
        glLineWidth(2.0f);
        
        glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, 0);
        glDrawArrays(GL_LINES, 0, lineData.size() / 3);
        glDisableClientState(GL_VERTEX_ARRAY);
    };

    inline virtual void drawTempPoints() {
        if (tempPointData.empty()) return;

        glColor3f(0.0f, 1.0f, 0.0f);  // Green
        glPointSize(10.0f);
        
        glBindBuffer(GL_ARRAY_BUFFER, tempPointVBO);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, 0);
        glDrawArrays(GL_POINTS, 0, tempPointData.size() / 3);
        glDisableClientState(GL_VERTEX_ARRAY);
    };

    inline virtual void drawTempLines() {
        if (tempLineData.empty()) return;

        glColor3f(0.0f, 1.0f, 0.0f);  // Green
        glLineWidth(0.5f);
        
        glBindBuffer(GL_ARRAY_BUFFER, tempLineVBO);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, 0);
        glDrawArrays(GL_LINES, 0, tempLineData.size() / 3);
        glDisableClientState(GL_VERTEX_ARRAY);
    };

    std::vector<float> pointData;
    std::vector<float> tempPointData;
    std::vector<float> lineData;
    std::vector<float> tempLineData;
    std::vector<float> gridData;

    float gridSize;
    
    // ----- Projection -----
    virtual void updateProjectionMatrix() = 0;

    QMatrix4x4 projectionMatrix;   
      
private:

};

#endif // VIEW_OPENGLWIDGET_H
