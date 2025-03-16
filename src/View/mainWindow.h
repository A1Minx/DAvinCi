#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
class Model;
class Controller;
class View_OpenGLWidget;

class mainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit mainWindow(QWidget *parent = nullptr);

private:
    Model *model;
    Controller *controller;
    View_OpenGLWidget *view;

    QGridLayout *mainLayout;

public slots:
    void setXYView();
    void setXZView();
    void setYZView();
};

#endif // WINDOW_H
