#include "mainWindow.h"
#include "Controller.h"
#include "Model.h"
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
 Controller *controller = new Controller(model, nullptr);
 DrawingOpenGLWidget *view = new DrawingOpenGLWidget(model, controller);
 controller->setView(view);  


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



 //----- test buttons -----
 //-- load
 QPushButton *writeSQL = new QPushButton("Write SQL",  this);
 writeSQL->setText("Write SQL");
 writeSQL->setToolTip("Write SQL");
 writeSQL->show();
 writeSQL->setIcon(QIcon::fromTheme("face-smile"));

 QObject::connect(writeSQL, SIGNAL(clicked()), controller, SLOT(writeSQL()));

 buttonLayout->addWidget(writeSQL);

 //-- save
  QPushButton *readSQL = new QPushButton("Read SQL",  this);
 readSQL->setText("Read SQL");
 readSQL->setToolTip("Read SQL");
 readSQL->show();
 readSQL->setIcon(QIcon::fromTheme("face-smile"));

 QObject::connect(readSQL, SIGNAL(clicked()), controller, SLOT(readSQL()));

 buttonLayout->addWidget(readSQL);
}
