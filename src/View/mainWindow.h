#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
class Model;
class Controller;
class View_OpenGLWidget;
class Orth_XY_OpenGLWidget;
class Orth_XZ_OpenGLWidget;
class Orth_YZ_OpenGLWidget;
class Free_OpenGLWidget;

class mainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit mainWindow(QWidget *parent = nullptr);

private:
    Model *model;
    Controller *controller;
    View_OpenGLWidget *view; // Current active view
    
    // Pre-allocated view instances
    Orth_XY_OpenGLWidget *xyView;
    Orth_XZ_OpenGLWidget *xzView;
    Orth_YZ_OpenGLWidget *yzView;
    Free_OpenGLWidget *freeView;

    QGridLayout *mainLayout;
    QLineEdit *horizonLineEdit;
    QLineEdit *gridPrecisionLineEdit;

    void createMenus();
    void createTopButtonBar();
    void createBottomButtonBar();

public slots:
    void setXYView();
    void setXZView();
    void setYZView();
    void setFreeView();
    void setHorizon();
    void setGridPrecision();
};

#endif // WINDOW_H
