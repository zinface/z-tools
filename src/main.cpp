#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QStyleFactory>
#include "version.h"

int main(int argc, char *argv[])
{
#ifdef Q_OS_MACOS
//    qDebug() << QStyleFactory::keys(); // Optional supported theme styles
    qApp->setStyle(QStyleFactory::create("fusion"));
#endif
    QApplication a(argc, argv);
    a.setApplicationName("z-tools-scanport");
    a.setApplicationDisplayName("ScanPort 端口扫描 - " PROJECT_VERSION);
    MainWindow wm;
    wm.show();
    QDesktopWidget *desktop = QApplication::desktop();
    wm.move((desktop->width() - wm.width()) /2,(desktop->height() - wm.height()) /2);
    return a.exec();
}
