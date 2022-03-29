#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
#ifdef Q_OS_MACOS
    qApp->setStyle(QStyleFactory::create("fusion"));
#endif
    QApplication a(argc, argv);
    a.setApplicationName("安卓信息查看器 - v0.1");
    MainWindow wm;
    wm.show();
    QDesktopWidget *desktop = QApplication::desktop();
    wm.move((desktop->width() - wm.width()) /2,(desktop->height() - wm.height()) /2);
    return a.exec();
}
