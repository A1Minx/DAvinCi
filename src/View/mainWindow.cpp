#include "mainWindow.h"
#include "Controller/Controller.h"
#include "Model/Model.h"
#include "DrawingOpenGLWidget.h"
#include <QPushButton>
#include <QApplication>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QOpenGLWidget>
#include <QPainter>



mainWindow::mainWindow(QWidget *parent) : QMainWindow(parent) {
 showMaximized();

 Model *model = new Model();
 DrawingOpenGLWidget *view = new DrawingOpenGLWidget(model);
 Controller *controller = new Controller(model, view);
 view->configModeController(controller->getModeController());


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

 // -- button row
 QHBoxLayout *buttonLayout = new QHBoxLayout();

 mainLayout->addLayout(buttonLayout,0,0,Qt::AlignTop);

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



 //----- basic functionality buttons -----
 //-- load
 QPushButton *loadButton = new QPushButton("DummyDraw",  this);
 loadButton->setText("DummyDraw");
 loadButton->setToolTip("DummyDraw");
 loadButton->show();
 loadButton->setIcon(QIcon::fromTheme("face-smile"));

 QObject::connect(loadButton, SIGNAL(clicked()), controller, SLOT(dummyDraw()));

 buttonLayout->addWidget(loadButton);

 //-- save
  QPushButton *saveButton = new QPushButton("Save",  this);
 saveButton->setText("Save");
 saveButton->setToolTip("Save File");
 saveButton->show();
 saveButton->setIcon(QIcon::fromTheme("face-smile"));

 QObject::connect(saveButton, SIGNAL(clicked()), controller, SLOT(save()));

 buttonLayout->addWidget(saveButton);
}
