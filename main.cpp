#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.showMaximized(); // makes the window start at full screen.
    w.show();

    return a.exec();
}
