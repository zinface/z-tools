#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
#ifdef Q_OS_MACOS
//    qDebug() << QStyleFactory::keys(); // Optional supported theme styles
    qApp->setStyle(QStyleFactory::create("fusion"));
#endif
    QApplication a(argc, argv);
    a.setApplicationName("z-Tools-ScanPort");
    MainWindow wm;
    wm.show();
    QDesktopWidget *desktop = QApplication::desktop();
    wm.move((desktop->width() - wm.width()) /2,(desktop->height() - wm.height()) /2);
    return a.exec();
}
