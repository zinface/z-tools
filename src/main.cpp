#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("桌面图标生成器 - v1.2.1");
    MainWindow wm;
    wm.show();
    QDesktopWidget *desktop = QApplication::desktop();
    wm.move((desktop->width() - wm.width()) /2,(desktop->height() - wm.height()) /2);
    return a.exec();
}
