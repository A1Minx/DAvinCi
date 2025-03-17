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
    QLineEdit *horizonLineEdit;
    QLineEdit *gridPrecisionLineEdit;

public slots:
    void setXYView();
    void setXZView();
    void setYZView();
    void setHorizon();
    void setGridPrecision();
};

#endif // WINDOW_H
