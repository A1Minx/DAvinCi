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
#include <QDebug>
#include <QTreeWidget>
#include <QHeaderView>
#include <vector>

// ----- Setter -----
// -- Set View --
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
  
  updateAxisButtonStyles();

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

// -- Set Horizon Axis --
void mainWindow::setHorizonAxisX() {
    currentHorizonAxis = 'x';
    view->setHorizonAxis('x');
    updateAxisButtonStyles();
}

void mainWindow::setHorizonAxisY() {
    currentHorizonAxis = 'y';
    view->setHorizonAxis('y');
    updateAxisButtonStyles();
}

void mainWindow::setHorizonAxisZ() {
    currentHorizonAxis = 'z';
    view->setHorizonAxis('z');
    updateAxisButtonStyles();
}

// -- Set Horizon --
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

void mainWindow::updateHorizon(float horizon) {
  if (view && horizonLineEdit) {
    horizonLineEdit->setText(QString::number(horizon));
    view->setHorizon(horizon);
  }
}


// ----- Menu -----
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

// ----- Top Buttons ----- (so far only for Debug)
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


 QPushButton *readSQLcomposedObjects = new QPushButton("Read SQL Composed Objects", this);
 readSQLcomposedObjects->setText("Read SQL Composed Objects");
 readSQLcomposedObjects->setToolTip("Read SQL Composed Objects");
 readSQLcomposedObjects->show();

 QObject::connect(readSQLcomposedObjects, SIGNAL(clicked()), controller, SLOT(readSQLRootComposedObjects()));

 buttonLayout->addWidget(readSQLcomposedObjects);
}

// ----- Button Interactivity -----
void mainWindow::updateAxisButtonStyles() {
    // Reset styles
    axisButtonX->setStyleSheet("");
    axisButtonY->setStyleSheet("");
    axisButtonZ->setStyleSheet("");
    
    // Set active button style
    QString activeStyle = "background-color: #3498db; color: white; font-weight: bold;";
    
    switch (currentHorizonAxis) {
        case 'x':
            axisButtonX->setStyleSheet(activeStyle);
            break;
        case 'y':
            axisButtonY->setStyleSheet(activeStyle);
            break;
        case 'z':
            axisButtonZ->setStyleSheet(activeStyle);
            break;
    }
}

// ----- Bottom Button Bar -----
void mainWindow::createBottomButtonBar() {
 QHBoxLayout *buttonLayoutBottom = new QHBoxLayout();
 buttonLayoutBottom->setAlignment(Qt::AlignLeft);

 mainLayout->addLayout(buttonLayoutBottom, 2, 0);

// ----- Views -----
// -- XY View --
 QPushButton *XYView = new QPushButton("XY View", this);
 XYView->setText("XY View");
 XYView->setToolTip("XY View");
 XYView->show();
 XYView->setFixedWidth(100);
 XYView->setFixedHeight(30);

 QObject::connect(XYView, SIGNAL(clicked()), this, SLOT(setXYView()));

 buttonLayoutBottom->addWidget(XYView);

// -- XZ View --
 QPushButton *XZView = new QPushButton("XZ View", this);
 XZView->setText("XZ View");
 XZView->setToolTip("XZ View");
 XZView->show();
 XZView->setFixedWidth(100);
 XZView->setFixedHeight(30);

 QObject::connect(XZView, SIGNAL(clicked()), this, SLOT(setXZView()));

 buttonLayoutBottom->addWidget(XZView);

// -- YZ View --
 QPushButton *YZView = new QPushButton("YZ View", this);
 YZView->setText("YZ View");
 YZView->setToolTip("YZ View");
 YZView->show();
 YZView->setFixedWidth(100);
 YZView->setFixedHeight(30);

 QObject::connect(YZView, SIGNAL(clicked()), this, SLOT(setYZView()));

 buttonLayoutBottom->addWidget(YZView);


// -- Free View --
//TODO: When not in free view, changing the axis button does only change the animation in GUI, not the Grid.
 QWidget *freeViewContainer = new QWidget(this);
 freeViewContainer->setFixedWidth(100);
 freeViewContainer->setFixedHeight(30);
 QVBoxLayout *freeViewLayout = new QVBoxLayout(freeViewContainer);
 freeViewLayout->setContentsMargins(0, 0, 0, 0);
 freeViewLayout->setSpacing(1);
 
 freeViewButton = new QPushButton("Free View", this);
 freeViewButton->setText("Free View");
 freeViewButton->setToolTip("Free View (Ctrl+Middle Mouse to tilt camera)");
 freeViewButton->show();
 QObject::connect(freeViewButton, SIGNAL(clicked()), this, SLOT(setFreeView()));
 
 freeViewLayout->addWidget(freeViewButton);
 
 QWidget *axisButtonsContainer = new QWidget(freeViewContainer);
 QHBoxLayout *axisLayout = new QHBoxLayout(axisButtonsContainer);
 axisLayout->setContentsMargins(0, 0, 0, 0);
 axisLayout->setSpacing(0);
 
 axisButtonX = new QPushButton("X", axisButtonsContainer);
 axisButtonY = new QPushButton("Y", axisButtonsContainer);
 axisButtonZ = new QPushButton("Z", axisButtonsContainer);
 
 axisButtonX->setToolTip("Use X as horizon axis (YZ plane grid)");
 axisButtonY->setToolTip("Use Y as horizon axis (XZ plane grid)");
 axisButtonZ->setToolTip("Use Z as horizon axis (XY plane grid)");
 
 axisButtonX->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
 axisButtonY->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
 axisButtonZ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
 
 axisButtonX->setFixedHeight(10);
 axisButtonY->setFixedHeight(10);
 axisButtonZ->setFixedHeight(10);
 
 QObject::connect(axisButtonX, SIGNAL(clicked()), this, SLOT(setHorizonAxisX()));
 QObject::connect(axisButtonY, SIGNAL(clicked()), this, SLOT(setHorizonAxisY()));
 QObject::connect(axisButtonZ, SIGNAL(clicked()), this, SLOT(setHorizonAxisZ()));
 
 axisLayout->addWidget(axisButtonX);
 axisLayout->addWidget(axisButtonY);
 axisLayout->addWidget(axisButtonZ);
 
 freeViewLayout->addWidget(axisButtonsContainer);
 
 buttonLayoutBottom->addWidget(freeViewContainer);

// ----- View Property edit -----
 QLabel *horizonLabel = new QLabel("Horizon:", this);
 buttonLayoutBottom->addWidget(horizonLabel);
 
 horizonLineEdit = new QLineEdit(this);
 horizonLineEdit->setFixedWidth(100);
 horizonLineEdit->setFixedHeight(30);
 horizonLineEdit->setText("100");
 horizonLineEdit->setToolTip("Standard Value for not visible Dimension");
 QObject::connect(horizonLineEdit, SIGNAL(editingFinished()), this, SLOT(setHorizon()));
 buttonLayoutBottom->addWidget(horizonLineEdit);
 
 QLabel *gridPrecisionLabel = new QLabel("Grid Precision:", this);
 buttonLayoutBottom->addWidget(gridPrecisionLabel);
 
 gridPrecisionLineEdit = new QLineEdit(this);
 gridPrecisionLineEdit->setText("100");
 gridPrecisionLineEdit->setToolTip("Grid Precision");
 gridPrecisionLineEdit->setFixedWidth(100);
 gridPrecisionLineEdit->setFixedHeight(30);

 QObject::connect(gridPrecisionLineEdit, SIGNAL(editingFinished()), this, SLOT(setGridPrecision()));
 buttonLayoutBottom->addWidget(gridPrecisionLineEdit);

 buttonLayoutBottom->addStretch(1);
}

// ----- Object Hierarchy Tree -----
void mainWindow::setupObjectHierarchyTree() {
  objectHierarchyTree = new QTreeWidget(this);
  objectHierarchyTree->setHeaderLabel("Objects");
  objectHierarchyTree->setMinimumWidth(200);
  objectHierarchyTree->setColumnCount(1);
  objectHierarchyTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
  objectHierarchyTree->setSelectionMode(QAbstractItemView::SingleSelection);
  objectHierarchyTree->setDragEnabled(true);
  objectHierarchyTree->setToolTip("Hierarchy of composed objects");
  
  // Connect to model's data change signals if available
  QObject::connect(controller, SIGNAL(composedObjectsChanged()), this, SLOT(updateObjectHierarchy()));
  
  // Setup item selection handling
  QObject::connect(objectHierarchyTree, &QTreeWidget::itemClicked, 
                  [this](QTreeWidgetItem* item, int column) {
                    // Handle selection - e.g., highlight the selected object in the view
                    // controller->selectObject(item->text(0));
                    qDebug() << "Selected object:" << item->text(0);
                  });
  
  mainLayout->addWidget(objectHierarchyTree, 1, 1);
  
  updateObjectHierarchy();
}

void mainWindow::addObjectToTree(std::shared_ptr<ComposedObject> obj, QTreeWidgetItem* parent) {
  if (!obj) {
    qDebug() << "Warning: Trying to add a null object to tree";
    return;
  }
  
  QString name = obj->getName().c_str();
  qDebug() << "Adding object to tree:" << name << (parent ? "with parent" : "as root");
  
  QTreeWidgetItem* item;
  
  if (parent == nullptr) {
    // For root level items, add directly to the tree widget
    item = new QTreeWidgetItem(objectHierarchyTree);
  } else {
    // For child items, add to the parent item
    item = new QTreeWidgetItem(parent);
  }
  
  // Set the name and make it expanded by default
  item->setText(0, name);
  item->setExpanded(true);

  std::vector<std::shared_ptr<ComposedObject>> children = obj->getChildren();
  qDebug() << "  Object has" << children.size() << "children";
  
  for (std::shared_ptr<ComposedObject> child : children) {
    addObjectToTree(child, item);
  }
}

void mainWindow::updateObjectHierarchy() {
  objectHierarchyTree->clear();

  std::vector<std::shared_ptr<ComposedObject>> rootObjects = model->getComposedObjects();
  
  qDebug() << "Object hierarchy updated: found" << rootObjects.size() << "root objects";

  for (std::shared_ptr<ComposedObject> obj : rootObjects) {
    addObjectToTree(obj, nullptr);
  }
}

// ----- Main Window -----
mainWindow::mainWindow(QWidget *parent) : QMainWindow(parent) {
 showMaximized();

 model = new Model();
 controller = new Controller(model, nullptr);
 currentHorizonAxis = 'z';

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
 
 QObject::connect(freeView, &Free_OpenGLWidget::horizonChanged, this, &mainWindow::updateHorizon);

 // ----- Object Hierarchy Widget -----
 setupObjectHierarchyTree();
}
