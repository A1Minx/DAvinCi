#ifndef VIEW_OPENGLWIDGET_H
#define VIEW_OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QVector3D>
#include <Model.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h> 
#include <errno.h>  
#include <string.h> 
#include <sys/stat.h> 
#include <cmath>

class Controller;

class View_OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit View_OpenGLWidget(Model *model, Controller *controller, QWidget *parent)
    : QOpenGLWidget(parent), model(model), controller(controller),
      zoomLevel(1.0f), 
      pointVBO(0), tempPointVBO(0), lineVBO(0), tempLineVBO(0), gridVBO(0),
      gridSize(100.0f), horizon(100.0f),
      isPanning(false), panOffsetX(0.0f), panOffsetY(0.0f),
      ctrlPressed(false)
    {
        setFocusPolicy(Qt::StrongFocus);
    }

    inline virtual void setHorizon(float horizon) {
        this->horizon = horizon;
        update();
    }

    inline virtual void setGridSize(float gridSize) {
        this->gridSize = gridSize;
        update();
    }

    inline virtual float getHorizon() {
        return horizon;
    }

    inline virtual float getGridSize() {
        return gridSize;
    }

    inline virtual void setHorizonAxis(char axis) {
        this->horizonAxis = axis;
        update();
    }
    inline virtual char getHorizonAxis() const {
        return horizonAxis;
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

        virtual QVector3D getCameraPosition() {
        return cameraPosition;
    }

    // ----- Camera Position -----
    virtual QVector3D getCameraTarget() {
        return cameraTarget;
    }
    virtual QVector3D getCameraUp() {
        return cameraUp;
    }
    virtual void setCameraPosition(QVector3D position) {
        cameraPosition = position;
    }
    virtual void setCameraTarget(QVector3D target) {
        cameraTarget = target;
    }
    virtual void setCameraUp(QVector3D up) {
        cameraUp = up;
    }


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

        // load identity
        projectionMatrix.setToIdentity();
        viewMatrix.setToIdentity();
        modelMatrix.setToIdentity();

        
        // Shader Compilation TODO: Check if this can be done in constructor, also, if shaders stay the same in all views, 
        // change view management to get set up once at startup.
        std::string vertexShaderPath = "../View/Shaders/vertex.shader";
        std::string fragmentShaderPath = "../View/Shaders/fragment.shader";
        
        std::string vertexShader = LoadShaderFromFile(vertexShaderPath);
        std::string fragmentShader = LoadShaderFromFile(fragmentShaderPath);
        
        if (!vertexShader.empty() && !fragmentShader.empty()) {
            shaderProgram = CreateShader(vertexShader, fragmentShader);
            std::cout << "Shaders compiled successfully" << std::endl;
        } else {
            std::cout << "Failed to load shaders" << std::endl;
        }

        setupBuffers();
    }

    inline virtual void resizeGL(int w, int h) {
        glViewport(0, 0, w, h); // TODO: Implement possibility to have multiple viewports in subclasses
        UpdateMatrices();
        update();
    }

    inline virtual void paintGL() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        // glMatrixMode(GL_PROJECTION);
        // glLoadIdentity();
        // glLoadMatrixf(projectionMatrix.constData());

        // glMatrixMode(GL_MODELVIEW);
        // glLoadIdentity();
        // glScalef(zoomLevel, zoomLevel, 1.0f);

        GLint Matrix_Projection = glGetUniformLocation(shaderProgram, "Matrix_Projection");
        GLint Matrix_View = glGetUniformLocation(shaderProgram, "Matrix_View");
        GLint Matrix_Model = glGetUniformLocation(shaderProgram, "Matrix_Model");

        glUseProgram(shaderProgram);

        glUniformMatrix4fv(Matrix_Projection, 1, GL_FALSE, projectionMatrix.constData());
        glUniformMatrix4fv(Matrix_View, 1, GL_FALSE, viewMatrix.constData());
        glUniformMatrix4fv(Matrix_Model, 1, GL_FALSE, modelMatrix.constData());

        updateBuffers();
        drawGrid();

        glClear(GL_DEPTH_BUFFER_BIT);

        drawPoints();  
        drawLines();    
        drawTempPoints();       
        drawTempLines(); 

        glUseProgram(0);
    }

    // ----- Events -----

    float zoomLevel; 
    bool isPanning;
    QPoint lastPanPosition;
    float panOffsetX;
    float panOffsetY;
    bool ctrlPressed;

    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

    virtual inline void wheelEvent(QWheelEvent *event) {
        float delta = event->angleDelta().y() / 120.0f;
        zoomLevel *= (1.0f + delta * 0.1f);
        zoomLevel = std::max(0.1f, std::min(1000.0f, zoomLevel));
        
        UpdateMatrices();
        update();
    }

    // ----- Buffers -----

    // -- Buffer ID Variables --
    GLuint pointVBO;
    GLuint tempPointVBO;
    GLuint lineVBO;
    GLuint tempLineVBO;
    GLuint gridVBO; 

    inline virtual void setupBuffers() {
        // -- Initialize Buffer Objects --
        glGenBuffers(1, &pointVBO);
        glGenBuffers(1, &tempPointVBO);
        glGenBuffers(1, &lineVBO);
        glGenBuffers(1, &tempLineVBO);
        glGenBuffers(1, &gridVBO);
    }

    inline virtual void updateBuffers() {
        //TODO: Check if for loops can be optimized for performance, think about adding temp elements to the VBOs for persistent elements
        qDebug() << "Updating buffers";
        updateComposedObjects(model->getRootComposedObjects());
        updateTempPoints(model->getTempPoints());
        updateTempLines(model->getTempLines());

        
    }

    inline void updateComposedObjects(std::vector<std::shared_ptr<ComposedObject>> composedObjects) {
        
        std::vector<std::shared_ptr<Point>> points;
        std::vector<std::shared_ptr<Line>> lines;
        std::vector<std::shared_ptr<ComposedObject>> children;

        for (const std::shared_ptr<ComposedObject>& composedObject : composedObjects) {
            qDebug() << "Updating composed object";
            if (composedObject) {
                std::vector<std::shared_ptr<Point>> composedPoints = composedObject->getPoints();
                if (!composedPoints.empty()) {
                    qDebug() << "Inserting points";
                    points.insert(points.end(), composedPoints.begin(), composedPoints.end());
                }
                std::vector<std::shared_ptr<Line>> composedLines = composedObject->getLines();
                if (!composedLines.empty()) {
                    qDebug() << "Inserting lines";
                    lines.insert(lines.end(), composedLines.begin(), composedLines.end());
                }
                std::vector<std::shared_ptr<ComposedObject>> composedChildren = composedObject->getChildren();
                if (!composedChildren.empty()) {
                    qDebug() << "Inserting children";
                    children.insert(children.end(), composedChildren.begin(), composedChildren.end());
                }
            }
        }

        qDebug() << "Updating points";
        if (!points.empty()) {
            updatePoints(points);
        }
        qDebug() << "Updating lines";
        if (!lines.empty()) {
            updateLines(lines);
        }
        qDebug() << "Updating children";
        if (!children.empty()) {
            //TODO: this overwrites the parent, fix that
            updateComposedObjects(children);
        }

    }

    inline void updatePoints(std::vector<std::shared_ptr<Point>> points) {
        // Update points
        std::vector<float> newPointData;
        for (const std::shared_ptr<Point>& point : points) {
            newPointData.push_back(point->getX());
            newPointData.push_back(point->getY());
            newPointData.push_back(point->getZ());
            newPointData.push_back(point->getColor()[0]);
            newPointData.push_back(point->getColor()[1]);
            newPointData.push_back(point->getColor()[2]);
            newPointData.push_back(point->getColor()[3]);
            newPointData.push_back(point->getSize());
        }
        if (newPointData != pointData) {
            pointData = newPointData;
            glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
            glBufferData(GL_ARRAY_BUFFER, pointData.size() * sizeof(float), pointData.data(), GL_DYNAMIC_DRAW);
        }
    }

    inline void updateTempPoints(std::vector<TempPoint> tempPoints) {
        // Update temporary points
        std::vector<float> newTempPointData;
        for (const TempPoint& point : tempPoints) {
            newTempPointData.push_back(point.x);
            newTempPointData.push_back(point.y);
            newTempPointData.push_back(point.z);
            newTempPointData.push_back(point.color[0]);
            newTempPointData.push_back(point.color[1]);
            newTempPointData.push_back(point.color[2]);
            newTempPointData.push_back(point.color[3]);
            newTempPointData.push_back(point.size);
        }
        if (newTempPointData != tempPointData) {
            tempPointData = newTempPointData;
            glBindBuffer(GL_ARRAY_BUFFER, tempPointVBO);
            glBufferData(GL_ARRAY_BUFFER, tempPointData.size() * sizeof(float), tempPointData.data(), GL_DYNAMIC_DRAW);
        }
    }

    inline void updateLines(std::vector<std::shared_ptr<Line>> lines) {
        // Update lines
        std::vector<float> newLineData;
        for (const std::shared_ptr<Line>& line : lines) {
            newLineData.push_back(line->getP1()->getX());
            newLineData.push_back(line->getP1()->getY());
            newLineData.push_back(line->getP1()->getZ());
            newLineData.push_back(line->getColor()[0]);
            newLineData.push_back(line->getColor()[1]);
            newLineData.push_back(line->getColor()[2]);
            newLineData.push_back(line->getColor()[3]);
            newLineData.push_back(line->getWidth());
            
            newLineData.push_back(line->getP2()->getX());
            newLineData.push_back(line->getP2()->getY());
            newLineData.push_back(line->getP2()->getZ());
            newLineData.push_back(line->getColor()[0]);
            newLineData.push_back(line->getColor()[1]);
            newLineData.push_back(line->getColor()[2]);
            newLineData.push_back(line->getColor()[3]);
            newLineData.push_back(line->getWidth());
        }
        if (newLineData != lineData) {
            lineData = newLineData;
            glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
            glBufferData(GL_ARRAY_BUFFER, lineData.size() * sizeof(float), lineData.data(), GL_DYNAMIC_DRAW);
        }
    }

    inline void updateTempLines(std::vector<TempLine> tempLines) {
        // Update temp lines
        std::vector<float> newTempLineData;
        for (const TempLine& line : tempLines) {
            newTempLineData.push_back(line.x1);
            newTempLineData.push_back(line.y1);
            newTempLineData.push_back(line.z1);
            newTempLineData.push_back(line.color[0]);
            newTempLineData.push_back(line.color[1]);
            newTempLineData.push_back(line.color[2]);
            newTempLineData.push_back(line.color[3]);
            newTempLineData.push_back(line.width);
            
            newTempLineData.push_back(line.x2);
            newTempLineData.push_back(line.y2);
            newTempLineData.push_back(line.z2);
            newTempLineData.push_back(line.color[0]);
            newTempLineData.push_back(line.color[1]);
            newTempLineData.push_back(line.color[2]);
            newTempLineData.push_back(line.color[3]);
            newTempLineData.push_back(line.width);
        }
        if (newTempLineData != tempLineData) {
            tempLineData = newTempLineData;
            glBindBuffer(GL_ARRAY_BUFFER, tempLineVBO);
            glBufferData(GL_ARRAY_BUFFER, tempLineData.size() * sizeof(float), tempLineData.data(), GL_DYNAMIC_DRAW);
        }
    }

    float gridColor[4] = {0.5f, 0.5f, 0.5f, 1.0f};
    float gridWidth = 1.0f;
    float gridExtend = 100000.0f;
    char horizonAxis = 'z'; 
    // TODO: make horizonAxis a variable that can be set in GUI to be able to change the grid in free view
    // parametric Version of grid
    inline virtual void GridData(char horizonAxis, float horizon, float gridSize, float width, float height, float gridWidth, float* color) {

        // keep a bit larger to prevent weird glitches on the edge of the view
        float left = -width;
        float right = width;
        float bottom = -height;
        float top = height;

        // rounding TODO: Check if necessary
        left = floor(left / gridSize) * gridSize;
        right = ceil(right / gridSize) * gridSize;
        bottom = floor(bottom / gridSize) * gridSize;
        top = ceil(top / gridSize) * gridSize;

        // grid data
        std::vector<float> newGridData;

        switch (horizonAxis) {
            case 'x': 
                for (float y = bottom; y <= top; y += gridSize) {
                    // position
                    newGridData.push_back(horizon);
                    newGridData.push_back(y);
                    newGridData.push_back(left);
                    // color
                    newGridData.push_back(color[0]);
                    newGridData.push_back(color[1]);
                    newGridData.push_back(color[2]);
                    newGridData.push_back(color[3]);
                    // width
                    newGridData.push_back(gridWidth);
                    // position
                    newGridData.push_back(horizon);
                    newGridData.push_back(y);
                    newGridData.push_back(right);
                    // color
                    newGridData.push_back(color[0]);
                    newGridData.push_back(color[1]);
                    newGridData.push_back(color[2]);
                    newGridData.push_back(color[3]);
                    // width
                    newGridData.push_back(gridWidth);
                }

                for (float z = bottom; z <= top; z += gridSize) {
                    // position
                    newGridData.push_back(horizon);
                    newGridData.push_back(left);
                    newGridData.push_back(z);
                    // color
                    newGridData.push_back(color[0]);
                    newGridData.push_back(color[1]);
                    newGridData.push_back(color[2]);
                    newGridData.push_back(color[3]);
                    // width
                    newGridData.push_back(gridWidth);
                    // position
                    newGridData.push_back(horizon);
                    newGridData.push_back(right);
                    newGridData.push_back(z);
                    // color
                    newGridData.push_back(color[0]);
                    newGridData.push_back(color[1]);
                    newGridData.push_back(color[2]);
                    newGridData.push_back(color[3]);
                    // width
                    newGridData.push_back(gridWidth);
                }

                // axis data TODO: check if necessary
                gridWidth *= 4; // larger width for axis
                // position
                newGridData.push_back(horizon);
                newGridData.push_back(left);
                newGridData.push_back(0.0f);
                // color
                newGridData.push_back(color[0]);
                newGridData.push_back(color[1]);
                newGridData.push_back(color[2]);
                newGridData.push_back(color[3]);
                // width
                newGridData.push_back(gridWidth);
                // position
                newGridData.push_back(horizon);
                newGridData.push_back(right);
                newGridData.push_back(0.0f);
                // color
                newGridData.push_back(color[0]);
                newGridData.push_back(color[1]);
                newGridData.push_back(color[2]);
                newGridData.push_back(color[3]);
                // width
                newGridData.push_back(gridWidth);
                // position
                newGridData.push_back(horizon);
                newGridData.push_back(0.0f);
                newGridData.push_back(top);
                // color
                newGridData.push_back(color[0]);
                newGridData.push_back(color[1]);
                newGridData.push_back(color[2]);
                newGridData.push_back(color[3]);
                // width
                newGridData.push_back(gridWidth);   
                // position
                newGridData.push_back(horizon);
                newGridData.push_back(0.0f);
                newGridData.push_back(bottom);
                // color
                newGridData.push_back(color[0]);
                newGridData.push_back(color[1]);
                newGridData.push_back(color[2]);
                newGridData.push_back(color[3]);
                // width
                newGridData.push_back(gridWidth);


                // Horizon Axis for free view
                // position
                newGridData.push_back(100000.0f);
                newGridData.push_back(0.0f);
                newGridData.push_back(0.0f);
                // color
                newGridData.push_back(color[0]);
                newGridData.push_back(color[1]);
                newGridData.push_back(color[2]);
                newGridData.push_back(color[3]);
                // width
                newGridData.push_back(gridWidth);   
                // position
                newGridData.push_back(-100000.0f);
                newGridData.push_back(0.0f);
                newGridData.push_back(0.0f);
                // color
                newGridData.push_back(color[0]);
                newGridData.push_back(color[1]);
                newGridData.push_back(color[2]);
                newGridData.push_back(color[3]);
                // width
                newGridData.push_back(gridWidth);

                //if( newGridData != gridData) {
                    gridData = newGridData;
                    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
                    glBufferData(GL_ARRAY_BUFFER, gridData.size() * sizeof(float), gridData.data(), GL_DYNAMIC_DRAW);
                //}

                break;
            case 'y':
                for (float x = left; x <= right; x += gridSize) {
                    // position
                    newGridData.push_back(x);
                    newGridData.push_back(horizon);
                    newGridData.push_back(bottom);
                    // color
                    newGridData.push_back(color[0]);
                    newGridData.push_back(color[1]);
                    newGridData.push_back(color[2]);
                    newGridData.push_back(color[3]);
                    // width
                    newGridData.push_back(gridWidth);
                    // position
                    newGridData.push_back(x);
                    newGridData.push_back(horizon);
                    newGridData.push_back(top);
                    // color
                    newGridData.push_back(color[0]);
                    newGridData.push_back(color[1]);
                    newGridData.push_back(color[2]);
                    newGridData.push_back(color[3]);
                    // width
                    newGridData.push_back(gridWidth);
                }

                for (float z = bottom; z <= top; z += gridSize) {
                    // position
                    newGridData.push_back(left);
                    newGridData.push_back(horizon);
                    newGridData.push_back(z);
                    // color
                    newGridData.push_back(color[0]);
                    newGridData.push_back(color[1]);
                    newGridData.push_back(color[2]);
                    newGridData.push_back(color[3]);
                    // width
                    newGridData.push_back(gridWidth);
                    // position
                    newGridData.push_back(right);
                    newGridData.push_back(horizon);
                    newGridData.push_back(z);
                    // color
                    newGridData.push_back(color[0]);
                    newGridData.push_back(color[1]);
                    newGridData.push_back(color[2]);
                    newGridData.push_back(color[3]);
                    // width
                    newGridData.push_back(gridWidth);
                }

                // axis data TODO: check if necessary
                gridWidth *= 4; // larger width for axis
                //TODO: implement shader for lines that implements line width (convert to triangle strips)

                // position
                newGridData.push_back(left);
                newGridData.push_back(horizon);
                newGridData.push_back(0.0f);
                // color
                newGridData.push_back(color[0]);
                newGridData.push_back(color[1]);
                newGridData.push_back(color[2]);
                newGridData.push_back(color[3]);
                // width
                newGridData.push_back(gridWidth);

                //position
                newGridData.push_back(right);
                newGridData.push_back(horizon);
                newGridData.push_back(0.0f);
                // color
                newGridData.push_back(color[0]);
                newGridData.push_back(color[1]);
                newGridData.push_back(color[2]);
                newGridData.push_back(color[3]);
                // width
                newGridData.push_back(gridWidth);
                
                //position
                newGridData.push_back(0.0f);
                newGridData.push_back(horizon);
                newGridData.push_back(top);
                // color
                newGridData.push_back(color[0]);
                newGridData.push_back(color[1]);
                newGridData.push_back(color[2]);
                newGridData.push_back(color[3]);
                // width
                newGridData.push_back(gridWidth);

                //position
                newGridData.push_back(0.0f);
                newGridData.push_back(horizon);
                newGridData.push_back(bottom);
                // color
                newGridData.push_back(color[0]);
                newGridData.push_back(color[1]);
                newGridData.push_back(color[2]);
                newGridData.push_back(color[3]);
                // width
                newGridData.push_back(gridWidth);


                // Horizon Axis for free view
                // position
                newGridData.push_back(0.0f);
                newGridData.push_back(100000.0f);
                newGridData.push_back(0.0f);
                // color
                newGridData.push_back(color[0]);
                newGridData.push_back(color[1]);
                newGridData.push_back(color[2]);
                newGridData.push_back(color[3]);
                // width
                newGridData.push_back(gridWidth);
                // position
                newGridData.push_back(0.0f);
                newGridData.push_back(-100000.0f);
                newGridData.push_back(0.0f);
                // color
                newGridData.push_back(color[0]);
                newGridData.push_back(color[1]);
                newGridData.push_back(color[2]);
                newGridData.push_back(color[3]);
                // width
                newGridData.push_back(gridWidth);
                
                //if( newGridData != gridData) {
                    gridData = newGridData;
                    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
                    glBufferData(GL_ARRAY_BUFFER, gridData.size() * sizeof(float), gridData.data(), GL_DYNAMIC_DRAW);
                //}
                
                break;
            case 'z':               
                for (float x = left; x <= right; x += gridSize) {
                    // position
                    newGridData.push_back(x);
                    newGridData.push_back(bottom);
                    newGridData.push_back(horizon);
                    // color
                    newGridData.push_back(color[0]);
                    newGridData.push_back(color[1]);
                    newGridData.push_back(color[2]);
                    newGridData.push_back(color[3]);
                    // width
                    newGridData.push_back(gridWidth);
                    
                    newGridData.push_back(x);
                    newGridData.push_back(top);
                    newGridData.push_back(horizon);
                    // color
                    newGridData.push_back(color[0]);
                    newGridData.push_back(color[1]);
                    newGridData.push_back(color[2]);
                    newGridData.push_back(color[3]);
                    // width
                    newGridData.push_back(gridWidth);
                }

                for (float y = bottom; y <= top; y += gridSize) {
                    // position
                    newGridData.push_back(left);
                    newGridData.push_back(y);
                    newGridData.push_back(horizon);
                    // color
                    newGridData.push_back(color[0]);
                    newGridData.push_back(color[1]);
                    newGridData.push_back(color[2]);
                    newGridData.push_back(color[3]);
                    // width
                    newGridData.push_back(gridWidth);
                    
                    newGridData.push_back(right);
                    newGridData.push_back(y);
                    newGridData.push_back(horizon);
                    // color
                    newGridData.push_back(color[0]);
                    newGridData.push_back(color[1]);
                    newGridData.push_back(color[2]);
                    newGridData.push_back(color[3]);
                    // width
                    newGridData.push_back(gridWidth);
                }

                // axis data TODO: check if necessary
                // position
                newGridData.push_back(left);
                newGridData.push_back(0.0f);
                newGridData.push_back(horizon);
                // color
                newGridData.push_back(color[0]);
                newGridData.push_back(color[1]);
                newGridData.push_back(color[2]);
                newGridData.push_back(color[3]);
                // width
                newGridData.push_back(gridWidth);

                //position
                newGridData.push_back(right);
                newGridData.push_back(0.0f);
                newGridData.push_back(horizon);
                // color
                newGridData.push_back(color[0]);
                newGridData.push_back(color[1]);
                newGridData.push_back(color[2]);
                newGridData.push_back(color[3]);
                // width
                newGridData.push_back(gridWidth);

                //position
                newGridData.push_back(0.0f);
                newGridData.push_back(bottom);
                newGridData.push_back(horizon);
                // color
                newGridData.push_back(color[0]);
                newGridData.push_back(color[1]);
                newGridData.push_back(color[2]);
                newGridData.push_back(color[3]);
                // width
                newGridData.push_back(gridWidth);
                
                //position
                newGridData.push_back(0.0f);
                newGridData.push_back(top);
                newGridData.push_back(horizon);
                // color
                newGridData.push_back(color[0]);
                newGridData.push_back(color[1]);
                newGridData.push_back(color[2]);
                newGridData.push_back(color[3]);
                // width
                newGridData.push_back(gridWidth);


                // Horizon Axis for free view
                // position
                newGridData.push_back(0.0f);
                newGridData.push_back(0.0f);
                newGridData.push_back(100000.0f);
                // color    
                newGridData.push_back(color[0]);
                newGridData.push_back(color[1]);
                newGridData.push_back(color[2]);
                newGridData.push_back(color[3]);
                // width
                newGridData.push_back(gridWidth);

                // position
                newGridData.push_back(0.0f);
                newGridData.push_back(0.0f);
                newGridData.push_back(-100000.0f);
                // color
                newGridData.push_back(color[0]);
                newGridData.push_back(color[1]);
                newGridData.push_back(color[2]);
                newGridData.push_back(color[3]);
                // width
                newGridData.push_back(gridWidth);
                              
                //if( newGridData != gridData) {
                    gridData = newGridData;
                    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
                    glBufferData(GL_ARRAY_BUFFER, gridData.size() * sizeof(float), gridData.data(), GL_DYNAMIC_DRAW);
                //}
                break;

            default:
                qDebug() << "Invalid horizon axis for Grid";
                return;
        }
        
        

    }

    // ----- Drawing -----
    inline virtual void drawGrid() {
        if (gridData.empty()) return;

        glBindBuffer(GL_ARRAY_BUFFER, gridVBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 7));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glDrawArrays(GL_LINES, 0, gridData.size() / 8);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        

        if (glGetError() != GL_NO_ERROR) {
            std::cout << "Shader Error" << std::endl;
        }
    }; 

    inline virtual void drawPoints() {
        if (pointData.empty()) return;
        
        glBindBuffer(GL_ARRAY_BUFFER, pointVBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);  
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 7));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    
        glDrawArrays(GL_POINTS, 0, pointData.size() / 8);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        if (glGetError() != GL_NO_ERROR) {
            std::cout << "Shader Error" << std::endl;
        }
    }

    inline virtual void drawLines() {
        if (lineData.empty()) return;
        
        glBindBuffer(GL_ARRAY_BUFFER, lineVBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);  
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 7));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glDrawArrays(GL_LINES, 0, lineData.size() / 8);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        if (glGetError() != GL_NO_ERROR) {
            std::cout << "Shader Error" << std::endl;
        }
    };

    inline virtual void drawTempPoints() {
        if (tempPointData.empty()) return;

        glBindBuffer(GL_ARRAY_BUFFER, tempPointVBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);  
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 7));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glDrawArrays(GL_POINTS, 0, tempPointData.size() / 8);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        if (glGetError() != GL_NO_ERROR) {
            std::cout << "Shader Error" << std::endl;
        }
    };

    inline virtual void drawTempLines() {
        if (tempLineData.empty()) return;

        glBindBuffer(GL_ARRAY_BUFFER, tempLineVBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);  
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 7));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glDrawArrays(GL_LINES, 0, tempLineData.size() / 8);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        if (glGetError() != GL_NO_ERROR) {
            std::cout << "Shader Error" << std::endl;
        }
    };

    std::vector<float> pointData;
    std::vector<float> tempPointData;
    std::vector<float> lineData;
    std::vector<float> tempLineData;
    std::vector<float> gridData;

    float gridSize;
    float horizon; 
    // TODO: Add specific Horizon for each Dimension and keep those consistent across views. 
    // Add "Origin" Coordinate that gets calculated at the Intersection of the horizon planes.

    // ----- Camera Position -----
    QVector3D cameraPosition;
    QVector3D cameraTarget;
    QVector3D cameraUp;

    // ----- Projection -----
    virtual void UpdateMatrices() = 0;

    QMatrix4x4 projectionMatrix;   
    QMatrix4x4 viewMatrix;
    QMatrix4x4 modelMatrix;



        // ----- Shader loading -----
    std::string LoadShaderFromFile(const std::string& filepath) {
        std::string filename = filepath;
        size_t lastSlash = filepath.find_last_of("/\\");
        if (lastSlash != std::string::npos) {
            filename = filepath.substr(lastSlash + 1);
        }
        
        std::string actualPath;
        if (filename == "vertex.shader") {
            actualPath = "../View/Shaders/vertex.shader";
        } else if (filename == "fragment.shader") {
            actualPath = "../View/Shaders/fragment.shader";
        } else {
            actualPath = filepath;
        }
        
        std::ifstream stream(actualPath);
        if (!stream.is_open()) {
            std::cerr << "Could not open Shader-File: " << actualPath << std::endl;
            return "";
        }
        
        std::string line;
        std::stringstream ss;
        while (std::getline(stream, line)) {
            ss << line << "\n";
        }
        
        std::cout << "Shader successfully loaded: " << actualPath << std::endl;
        return ss.str();
    }
    
    inline virtual unsigned int CompileShader(unsigned int type, const std::string& source)
    {        
        // compile the source code from the string "source", return the id to the compiled shader
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        this->glShaderSource(id, 1, &src, nullptr);
        this->glCompileShader(id);
    
        // check for compilation errors
        int result;
        this->glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) 
        {
            int length;
            this->glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)alloca(length * sizeof(char));
            this->glGetShaderInfoLog(id, length, &length, message);
            std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
            std::cout << message << std::endl;
            this->glDeleteShader(id);
            return 0;
        }
    
        return id;
    }
    
    inline virtual unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) 
    {        
        // compile shaders and link them as program
        unsigned int program = glCreateProgram();
        unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
        unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
        
        this->glAttachShader(program, vs);
        this->glAttachShader(program, fs);
        
        this->glLinkProgram(program);
    
        this->glValidateProgram(program);
    
        // remove shaders after linking program, may need to be kept for debugging in the future
        this->glDeleteShader(vs);
        this->glDeleteShader(fs);
    
        return program;
    }
      
private:

    GLuint shaderProgram;

};

#endif // VIEW_OPENGLWIDGET_H
