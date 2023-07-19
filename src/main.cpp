
#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QMainWindow>
#include <aptmanager.h>

#include "config.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("软件包管理器 - v" PROJECT_VERSION);
    AptManager wm;
    wm.show();
    QDesktopWidget *desktop = QApplication::desktop();
    wm.move((desktop->width() - wm.width()) /2,(desktop->height() - wm.height()) /2);
    return a.exec();
}
