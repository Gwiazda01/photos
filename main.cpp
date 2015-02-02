#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    MainWindow *w;
    QApplication a(argc, argv);
    w = new MainWindow();
    w->showMaximized();

    return a.exec();
}
