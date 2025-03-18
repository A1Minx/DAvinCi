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
#include <unistd.h> // Für getcwd
#include <errno.h>  // Für errno
#include <string.h> // Für strerror
#include <sys/stat.h> // Für stat

class Controller;

class View_OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit View_OpenGLWidget(Model *model, Controller *controller, QWidget *parent)
    : QOpenGLWidget(parent), model(model), controller(controller),
      zoomLevel(1.0f), 
      pointVBO(0), tempPointVBO(0), lineVBO(0), tempLineVBO(0), gridVBO(0),
      gridSize(100.0f)
    {
        setFocusPolicy(Qt::StrongFocus);
    }

    inline virtual void setHorizon(float horizon) {
        this->horizon = horizon;
    }

    inline virtual void setGridSize(float gridSize) {
        this->gridSize = gridSize;
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

        // load identity
        projectionMatrix.setToIdentity();
        viewMatrix.setToIdentity();
        modelMatrix.setToIdentity();

        //std::string basePath = getcwd(cwd, sizeof(cwd));
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
        updateProjectionMatrix();
        update();
    }

    inline virtual void paintGL() {
        glClear(GL_COLOR_BUFFER_BIT);
    
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glLoadMatrixf(projectionMatrix.constData());

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glScalef(zoomLevel, zoomLevel, 1.0f);

        updateBuffers();
        drawGrid();
        drawPoints();  
        drawLines();    
        drawTempPoints();       
        drawTempLines(); 
    }

    // ----- Events -----

    float zoomLevel; 

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
    }

    // ----- Drawing -----
    inline virtual void drawGrid() {
        if (gridData.empty()) return;

        glColor3f(0.3f, 0.3f, 0.3f);  // Dark Grey
        glLineWidth(1.0f);

        glBindBuffer(GL_ARRAY_BUFFER, gridVBO);

        // Verwende Shader
        glUseProgram(shaderProgram);
        
        // Setze Uniform-Variablen für Matrizen
        GLint Matrix_Projection = glGetUniformLocation(shaderProgram, "Matrix_Projection");
        GLint Matrix_View = glGetUniformLocation(shaderProgram, "Matrix_View");
        GLint Matrix_Model = glGetUniformLocation(shaderProgram, "Matrix_Model");

        glUniformMatrix4fv(Matrix_Projection, 1, GL_FALSE, projectionMatrix.constData());
        glUniformMatrix4fv(Matrix_View, 1, GL_FALSE, viewMatrix.constData());
        glUniformMatrix4fv(Matrix_Model, 1, GL_FALSE, modelMatrix.constData());

        // Zeichne mit Vertex Attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_LINES, 0, gridData.size() / 3);
        glDisableVertexAttribArray(0);
        
        // Deaktiviere Shader
        glUseProgram(0);

        // Alternativ: Falls Shader nicht funktionieren, verwende die alte Fixed-Function-Pipeline
        if (glGetError() != GL_NO_ERROR) {
            std::cout << "Fehler beim Rendering mit Shadern, verwende Fixed-Function-Pipeline" << std::endl;
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, 0, 0);
            glDrawArrays(GL_LINES, 0, gridData.size() / 3);
            glDisableClientState(GL_VERTEX_ARRAY);
        }
    }; 
    // TODO:Make sure grid is geometrically aligned to World instead of screen.

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
    float horizon;
    
    // ----- Projection -----
    virtual void updateProjectionMatrix() = 0;

    QMatrix4x4 projectionMatrix;   
    QMatrix4x4 viewMatrix;
    QMatrix4x4 modelMatrix;
      
private:

    GLuint shaderProgram;

};

#endif // VIEW_OPENGLWIDGET_H
