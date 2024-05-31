#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("图片浏览器 - v0.3");

    MainWindow wm;
    wm.show();

    return a.exec();
}
