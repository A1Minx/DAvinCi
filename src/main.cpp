#include <QApplication>
#include "./View/mainWindow.h"


int main(int argc, char **argv)
{
 QApplication app (argc, argv);

 mainWindow window;

 window.show();
 return app.exec();
}
