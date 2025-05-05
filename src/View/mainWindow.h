#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QTreeWidget>
#include "ComposedObject.h"

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
    mainWindow(QWidget *parent = nullptr);
    void updateHorizon(float horizon);

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
    
    // Buttons for free view axis control
    QPushButton *freeViewButton;
    QPushButton *axisButtonX;
    QPushButton *axisButtonY;
    QPushButton *axisButtonZ;
    char currentHorizonAxis;
    
    // Tree widget for object hierarchy
    QTreeWidget *objectHierarchyTree;
    
    void updateAxisButtonStyles();
    void createMenus();
    void createTopButtonBar();
    void createBottomButtonBar();
    void setupObjectHierarchyTree();

public slots:
    void setXYView();
    void setXZView();
    void setYZView();
    void setFreeView();
    void setHorizon();
    void setGridPrecision();
    void setHorizonAxisX();
    void setHorizonAxisY();
    void setHorizonAxisZ();
    
    void addObjectToTree(std::shared_ptr<ComposedObject> obj, QTreeWidgetItem* parent);
    void updateObjectHierarchy();
};

#endif // MAINWINDOW_H
