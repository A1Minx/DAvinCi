#include "mainWindow.h"
#include "Controller.h"
#include "Model.h"
#include "View_OpenGLWidget.h"
#include "Orth_XY_OpenGLWidget.h"
#include "Orth_XZ_OpenGLWidget.h"
#include "Orth_YZ_OpenGLWidget.h"
#include <QPushButton>
#include <QApplication>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QOpenGLWidget>
#include <QPainter>
#include <QLineEdit>
#include <QLabel>

void mainWindow::setXYView() {
  if (view) {
    view->hide();
    delete view;
  }
  
  
  view = new Orth_XY_OpenGLWidget(model, controller);
  controller->setView(view);
  controller->setHiddenAxis('z');
  
  mainLayout->addWidget(view, 1, 0);
  
  view->show();
  
  controller->getModeController()->reConfigureView();
 };

void mainWindow::setXZView() {
  if (view) {
    view->hide();
    delete view;
  }
  
  view = new Orth_XZ_OpenGLWidget(model, controller);
  controller->setView(view);
  controller->setHiddenAxis('y');
  mainLayout->addWidget(view, 1, 0);
  view->show();

  controller->getModeController()->reConfigureView();
 };

void mainWindow::setYZView() {
  if (view) {
    view->hide();
    delete view;
  }

  view = new Orth_YZ_OpenGLWidget(model, controller);
  controller->setView(view);
  controller->setHiddenAxis('x');
  mainLayout->addWidget(view, 1, 0);
  view->show();

  controller->getModeController()->reConfigureView();
 };

void mainWindow::setHorizon() {
  if (view && horizonLineEdit) {
    bool ok;
    float horizonValue = horizonLineEdit->text().toFloat(&ok);
    if (ok && horizonValue > 0) {
      view->setHorizon(horizonValue);
    }
  }
}

void mainWindow::setGridPrecision() {
  if (view && gridPrecisionLineEdit) {
    bool ok;
    float precisionValue = gridPrecisionLineEdit->text().toFloat(&ok);
    if (ok && precisionValue > 0) {
      view->setGridSize(precisionValue);
    }
  }
}

mainWindow::mainWindow(QWidget *parent) : QMainWindow(parent) {
 showMaximized();

 model = new Model();
 controller = new Controller(model, nullptr);
 view = nullptr;

  // ----- Layout -----
 QWidget *centralWidget = new QWidget(this);
 setCentralWidget(centralWidget);


 mainLayout = new QGridLayout(centralWidget);
 mainLayout->setContentsMargins(10, 10, 10, 10);
 mainLayout->setSpacing(5);

 setXYView();
 controller->setView(view);


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

 // -- button row to
 QHBoxLayout *buttonLayout = new QHBoxLayout();
 buttonLayout->setAlignment(Qt::AlignLeft);

 mainLayout->addLayout(buttonLayout, 0, 0);

 // -- button row bottom
 QHBoxLayout *buttonLayoutBottom = new QHBoxLayout();
 buttonLayoutBottom->setAlignment(Qt::AlignLeft);

 mainLayout->addLayout(buttonLayoutBottom, 2, 0);

 // -- vertical stretch
 mainLayout->setColumnStretch(0, 5); // View
 mainLayout->setColumnStretch(1, 1); // Side Container

 // -- horizontal stretch
 mainLayout->setRowStretch(0, 0);    // Top Buttons
 mainLayout->setRowStretch(1, 10);   // View
 mainLayout->setRowStretch(2, 0);    // Bottom Buttons

 // ----- Graphics Window -----
 mainLayout->addWidget(view, 1, 0);

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

 QPushButton *YZView = new QPushButton("YZ View",  this);
 YZView->setText("YZ View");
 YZView->setToolTip("YZ View");
 YZView->show();

 QObject::connect(YZView, SIGNAL(clicked()), this, SLOT(setYZView()));

 buttonLayoutBottom->addWidget(YZView);


 
// inputs for horizon and grid    
 QLabel *horizonLabel = new QLabel("Horizon:", this);
 buttonLayoutBottom->addWidget(horizonLabel);
 
 horizonLineEdit = new QLineEdit(this);
 horizonLineEdit->setFixedWidth(100);
 horizonLineEdit->setText("100");
 horizonLineEdit->setToolTip("Standard Value for not visible Dimension");
 QObject::connect(horizonLineEdit, SIGNAL(editingFinished()), this, SLOT(setHorizon()));
 buttonLayoutBottom->addWidget(horizonLineEdit);
 
 QLabel *gridPrecisionLabel = new QLabel("Grid Precision:", this);
 buttonLayoutBottom->addWidget(gridPrecisionLabel);
 
 gridPrecisionLineEdit = new QLineEdit(this);
 gridPrecisionLineEdit->setFixedWidth(100);
 gridPrecisionLineEdit->setText("100");
 gridPrecisionLineEdit->setToolTip("Grid Precision");
 QObject::connect(gridPrecisionLineEdit, SIGNAL(editingFinished()), this, SLOT(setGridPrecision()));
 buttonLayoutBottom->addWidget(gridPrecisionLineEdit);

  buttonLayoutBottom->addStretch(1);
}
