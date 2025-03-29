#include "mainWindow.h"
#include "Controller.h"
#include "Model.h"
#include "View_OpenGLWidget.h"
#include "Orth_XY_OpenGLWidget.h"
#include "Orth_XZ_OpenGLWidget.h"
#include "Orth_YZ_OpenGLWidget.h"
#include "Free_OpenGLWidget.h"
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
  }
  
  view = xyView;
  controller->setView(view);
  controller->setHiddenAxis('z');
  
  view->show();

  gridPrecisionLineEdit->setText(QString::number(view->getGridSize()));
  horizonLineEdit->setText(QString::number(view->getHorizon()));

  controller->getModeController()->reConfigureView();
 };

void mainWindow::setXZView() {
  if (view) {
    view->hide();
  }
  
  view = xzView;
  controller->setView(view);
  controller->setHiddenAxis('y');
  
  view->show();

  gridPrecisionLineEdit->setText(QString::number(view->getGridSize()));
  horizonLineEdit->setText(QString::number(view->getHorizon()));

  controller->getModeController()->reConfigureView();
 };

void mainWindow::setYZView() {
  if (view) {
    view->hide();
  }

  view = yzView;
  controller->setView(view);
  controller->setHiddenAxis('x');
  
  view->show();

  gridPrecisionLineEdit->setText(QString::number(view->getGridSize()));
  horizonLineEdit->setText(QString::number(view->getHorizon()));

  controller->getModeController()->reConfigureView();
 };

void mainWindow::setFreeView() {
  if (view) {
    view->hide();
  }

  //TODO: Make the switch more smooth, make sure the camera actuallie points the same way as the current view
  //TODO: Right now it only switches when the middle mouse is clicked and mouse not moving, check this
  QVector3D cameraPosition = view->getCameraPosition();
  QVector3D cameraTarget = view->getCameraTarget();
  QVector3D cameraUp = view->getCameraUp();
  qDebug() << "Camera Position old view: " << cameraPosition;
  qDebug() << "Camera Target old view: " << cameraTarget;
  qDebug() << "Camera Up old view: " << cameraUp;

  view = freeView;
  view->setCameraPosition(cameraPosition);
  view->setCameraTarget(cameraTarget);
  view->setCameraUp(cameraUp);

  qDebug() << "Camera Position new view: " << view->getCameraPosition();
  qDebug() << "Camera Target new view: " << view->getCameraTarget();
  qDebug() << "Camera Up new view: " << view->getCameraUp();

  controller->setView(view);
  controller->setHiddenAxis('0');
  view->show();

  // needed in free view to place grid position
  gridPrecisionLineEdit->setText(QString::number(view->getGridSize()));
  horizonLineEdit->setText(QString::number(view->getHorizon()));

  controller->getModeController()->reConfigureView();
}

void mainWindow::setHorizon() {
  if (view && horizonLineEdit) {
    bool ok;
    float horizonValue = horizonLineEdit->text().toFloat(&ok);
    if (ok) {
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

void mainWindow::createMenus() {
 QMenuBar *menuBar = new QMenuBar(this);
 setMenuBar(menuBar);
 QMenu *fileMenu = menuBar->addMenu("File");
 QAction *loadAction = fileMenu->addAction("Load File");
 QAction *saveAction = fileMenu->addAction("Save File");

 QObject::connect(loadAction, SIGNAL(triggered()), controller, SLOT(load()));
 QObject::connect(saveAction, SIGNAL(triggered()), controller, SLOT(save()));

 QMenu *toolsMenu = menuBar->addMenu("Tools");
 QAction *drawLineAction = toolsMenu->addAction("Draw Line");
 QAction *drawPointAction = toolsMenu->addAction("Draw Point");
 QAction *clearShapesAction = toolsMenu->addAction("Clear Shapes");
 QObject::connect(drawLineAction, SIGNAL(triggered()), controller, SLOT(setModeDrawLine()));
 QObject::connect(drawPointAction, SIGNAL(triggered()), controller, SLOT(setModeDrawPoint()));
 QObject::connect(clearShapesAction, SIGNAL(triggered()), controller, SLOT(clearShapes()));

 QMenu *advancedMenu = toolsMenu->addMenu("Advanced");
 QAction *subOption = advancedMenu->addAction("Advanced Option");
}

void mainWindow::createTopButtonBar() {
 QHBoxLayout *buttonLayout = new QHBoxLayout();
 buttonLayout->setAlignment(Qt::AlignLeft);

 mainLayout->addLayout(buttonLayout, 0, 0);

 QPushButton *readSQLLines = new QPushButton("Read SQL Lines", this);
 readSQLLines->setText("Read SQL Lines");
 readSQLLines->setToolTip("Read SQL Lines");
 readSQLLines->show();

 QObject::connect(readSQLLines, SIGNAL(clicked()), controller, SLOT(readSQLLines()));

 buttonLayout->addWidget(readSQLLines);

 QPushButton *readSQLPoints = new QPushButton("Read SQL Points", this);
 readSQLPoints->setText("Read SQL Points");
 readSQLPoints->setToolTip("Read SQL Points");
 readSQLPoints->show();

 QObject::connect(readSQLPoints, SIGNAL(clicked()), controller, SLOT(readSQLPoints()));

 buttonLayout->addWidget(readSQLPoints);
}

void mainWindow::createBottomButtonBar() {
 QHBoxLayout *buttonLayoutBottom = new QHBoxLayout();
 buttonLayoutBottom->setAlignment(Qt::AlignLeft);

 mainLayout->addLayout(buttonLayoutBottom, 2, 0);

 QPushButton *XYView = new QPushButton("XY View", this);
 XYView->setText("XY View");
 XYView->setToolTip("XY View");
 XYView->show();

 QObject::connect(XYView, SIGNAL(clicked()), this, SLOT(setXYView()));

 buttonLayoutBottom->addWidget(XYView);

 QPushButton *XZView = new QPushButton("XZ View", this);
 XZView->setText("XZ View");
 XZView->setToolTip("XZ View");
 XZView->show();

 QObject::connect(XZView, SIGNAL(clicked()), this, SLOT(setXZView()));

 buttonLayoutBottom->addWidget(XZView);

 QPushButton *YZView = new QPushButton("YZ View", this);
 YZView->setText("YZ View");
 YZView->setToolTip("YZ View");
 YZView->show();

 QObject::connect(YZView, SIGNAL(clicked()), this, SLOT(setYZView()));

 buttonLayoutBottom->addWidget(YZView);

 QPushButton *FreeView = new QPushButton("Free View", this);
 FreeView->setText("Free View");
 FreeView->setToolTip("Free View (Ctrl+Middle Mouse to tilt camera)");
 FreeView->show();

 QObject::connect(FreeView, SIGNAL(clicked()), this, SLOT(setFreeView()));

 buttonLayoutBottom->addWidget(FreeView);

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

mainWindow::mainWindow(QWidget *parent) : QMainWindow(parent) {
 showMaximized();

 model = new Model();
 controller = new Controller(model, nullptr);

  // ----- Layout -----
 QWidget *centralWidget = new QWidget(this);
 setCentralWidget(centralWidget);

 mainLayout = new QGridLayout(centralWidget);
 mainLayout->setContentsMargins(10, 10, 10, 10);
 mainLayout->setSpacing(5);

 // -- vertical stretch
 mainLayout->setColumnStretch(0, 5); // View
 mainLayout->setColumnStretch(1, 1); // Side Container

 // -- horizontal stretch
 mainLayout->setRowStretch(0, 0);    // Top Buttons
 mainLayout->setRowStretch(1, 10);   // View
 mainLayout->setRowStretch(2, 0);    // Bottom Buttons

 qDebug() << "Creating UI";
 createMenus();
 createTopButtonBar();
 createBottomButtonBar();

 qDebug() << "Creating views";
 xyView = new Orth_XY_OpenGLWidget(model, controller);
 xzView = new Orth_XZ_OpenGLWidget(model, controller);
 yzView = new Orth_YZ_OpenGLWidget(model, controller);
 freeView = new Free_OpenGLWidget(model, controller);
 
 xyView->hide();
 xzView->hide();
 yzView->hide();
 freeView->hide();

 mainLayout->addWidget(xyView, 1, 0);
 mainLayout->addWidget(xzView, 1, 0);
 mainLayout->addWidget(yzView, 1, 0);
 mainLayout->addWidget(freeView, 1, 0);

 view = nullptr;

 setXYView();



 // ----- Layers Widget -----
 QWidget *emptyContainer = new QWidget(this);
 emptyContainer->setMinimumWidth(200);
 emptyContainer->setStyleSheet("border: 1px solid red;");
 mainLayout->addWidget(emptyContainer, 1, 1);
}
