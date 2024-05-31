#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("z-tools-fake-component");

    MainWindow wm;
    wm.show();

    return a.exec();
}
