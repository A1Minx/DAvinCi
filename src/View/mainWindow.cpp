#include "mainWindow.h"
#include "Controller.h"
#include "Model.h"
#include "Orth_XY_OpenGLWidget.h"
#include "Orth_XZ_OpenGLWidget.h"
#include <QPushButton>
#include <QApplication>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QOpenGLWidget>
#include <QPainter>

void mainWindow::setXYView() {
  if (view) {
    view->hide();
    delete view;
  }
  
  view = new Orth_XY_OpenGLWidget(model, controller);
  controller->setView(static_cast<Orth_XY_OpenGLWidget*>(view));

//TODO: Use polymorphism instead of casting, check which view functions can be moved to parent class
  
  // Füge die View wieder zum Layout hinzu
  QGridLayout *mainLayout = qobject_cast<QGridLayout*>(centralWidget()->layout());
  if (mainLayout) {
    mainLayout->addWidget(view, 1, 0);
  }
  view->show();
 };

void mainWindow::setXZView() {
  if (view) {
    view->hide();
    delete view;
  }
  
  view = new Orth_XZ_OpenGLWidget(model, controller);
  controller->setView(static_cast<Orth_XZ_OpenGLWidget*>(view));
  
  // Füge die View wieder zum Layout hinzu
  QGridLayout *mainLayout = qobject_cast<QGridLayout*>(centralWidget()->layout());
  if (mainLayout) {
    mainLayout->addWidget(view, 1, 0);
  }
  view->show();
 };


mainWindow::mainWindow(QWidget *parent) : QMainWindow(parent) {
 showMaximized();

 model = new Model();
 controller = new Controller(model, nullptr);
 view = new Orth_XY_OpenGLWidget(model, controller);
 controller->setView(static_cast<Orth_XY_OpenGLWidget*>(view));


 QWidget *centralWidget = new QWidget(this);

 setCentralWidget(centralWidget);

 // ----- Menu -----
 // -- File
 QMenuBar *menuBar = new QMenuBar(this);
 setMenuBar(menuBar);
 QMenu *fileMenu = menuBar->addMenu("File");
 QAction *loadAction = fileMenu->addAction("Load File");
 QAction *saveAction = fileMenu->addAction("Save File");

 QObject::connect(loadAction, SIGNAL(triggered()), controller, SLOT(load()));
 QObject::connect(saveAction, SIGNAL(triggered()), controller, SLOT(save()));

 // -- Tools
 QMenu *toolsMenu = menuBar->addMenu("Tools");
 QAction *drawLineAction = toolsMenu->addAction("Draw Line");
 QAction *drawPointAction = toolsMenu->addAction("Draw Point");
 QAction *clearShapesAction = toolsMenu->addAction("Clear Shapes");
 //TODO: Parametrizemode selection with argument? Enum?
 QObject::connect(drawLineAction, SIGNAL(triggered()), controller, SLOT(setModeDrawLine()));
 QObject::connect(drawPointAction, SIGNAL(triggered()), controller, SLOT(setModeDrawPoint()));
 QObject::connect(clearShapesAction, SIGNAL(triggered()), controller, SLOT(clearShapes()));

 QMenu *advancedMenu = toolsMenu->addMenu("Advanced");
 QAction *subOption = advancedMenu->addAction("Advanced Option");


 // ----- Layout -----
 QGridLayout *mainLayout = new QGridLayout(centralWidget);

 // -- button row to
 QHBoxLayout *buttonLayout = new QHBoxLayout();

 mainLayout->addLayout(buttonLayout,0,0,Qt::AlignTop);

 // -- button row bottom
 QHBoxLayout *buttonLayoutBottom = new QHBoxLayout();

 mainLayout->addLayout(buttonLayoutBottom,1,0,Qt::AlignBottom);

 // -- vertical stretch
 mainLayout->setColumnStretch(0, 1);
 mainLayout->setColumnStretch(1, 0);

 // -- horizontal stretch
 mainLayout->setRowStretch(0, 0);
 mainLayout->setRowStretch(1, 1);


 // ----- Graphics Window -----

 mainLayout->addWidget(view);

 // ----- Layers Widget -----
 QWidget *emptyContainer = new QWidget(this);
 emptyContainer->setMinimumWidth(200);
 emptyContainer->setStyleSheet("border: 1px solid red;");
 mainLayout->addWidget(emptyContainer, 1, 1);



 //-----  buttons top -----
 //-- load
 QPushButton *readSQLLines = new QPushButton("Read SQL Lines",  this);
 readSQLLines->setText("Read SQL Lines");
 readSQLLines->setToolTip("Read SQL Lines");
 readSQLLines->show();
 //readSQLLines->setIcon(QIcon::fromTheme("face-smile"));

 QObject::connect(readSQLLines, SIGNAL(clicked()), controller, SLOT(readSQLLines()));

 buttonLayout->addWidget(readSQLLines);

 //-- save
  QPushButton *readSQLPoints = new QPushButton("Read SQL Points",  this);
 readSQLPoints->setText("Read SQL Points");
 readSQLPoints->setToolTip("Read SQL Points");
 readSQLPoints->show();
 //readSQLPoints->setIcon(QIcon::fromTheme("face-smile"));

 QObject::connect(readSQLPoints, SIGNAL(clicked()), controller, SLOT(readSQLPoints()));

 buttonLayout->addWidget(readSQLPoints);


 //-----  buttons bottom -----

 QPushButton *XYView = new QPushButton("XY View",  this);
 XYView->setText("XY View");
 XYView->setToolTip("XY View");
 XYView->show();

 QObject::connect(XYView, SIGNAL(clicked()), this, SLOT(setXYView()));

 buttonLayoutBottom->addWidget(XYView);

 QPushButton *XZView = new QPushButton("XZ View",  this);
 XZView->setText("XZ View");
 XZView->setToolTip("XZ View");
 XZView->show();

 QObject::connect(XZView, SIGNAL(clicked()), this, SLOT(setXZView()));

 buttonLayoutBottom->addWidget(XZView);
}
