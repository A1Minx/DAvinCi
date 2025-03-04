TEMPLATE = app
TARGET = DAvinCi

QT = core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets gui

LIBS *= -lpq

SOURCES +=  main.cpp ./View/mainWindow.cpp ./Controller/Controller.cpp ./View/DrawingOpenGLWidget.cpp ./Model/Model.cpp ./Controller/ModeController.cpp ./Controller/M_DrawLine.cpp ./Controller/M_DrawPoint.cpp ./Controller/M_Selection.cpp ./Model/SQLServer.cpp

HEADERS += ./View/mainWindow.h ./Controller/Controller.h ./View/DrawingOpenGLWidget.h ./Model/Model.h ./Controller/ModeController.h ./Controller/M_DrawLine.h ./Controller/M_ModeInterface.h ./Controller/M_DrawPoint.h ./Controller/M_Selection.h ./Model/SQLServer.h

INCLUDEPATH +=  ./Controller ./View ./Model /usr/include /usr/lib/x86_64-linux-gnu

CONFIG += c++17
