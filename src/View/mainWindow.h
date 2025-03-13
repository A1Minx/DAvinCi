#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

class Model;
class Controller;
class Orth_XY_OpenGLWidget;
class Orth_XZ_OpenGLWidget;

class mainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit mainWindow(QWidget *parent = nullptr);

private:
    Model *model;
    Controller *controller;
    QWidget *view;

public slots:
    void setXYView();
    void setXZView();
};

#endif // WINDOW_H
