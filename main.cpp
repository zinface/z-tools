#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
#ifdef Q_OS_MACOS
    qApp->setStyle(QStyleFactory::create("Fusion"));
#endif
    QApplication a(argc, argv);
    a.setApplicationName("apk文件查看器 - v1.0");
    MainWindow wm;
    wm.show();
    QDesktopWidget *desktop = QApplication::desktop();
    wm.move((desktop->width() - wm.width()) /2,(desktop->height() - wm.height()) /2);

    // 对可能传入的 apk 文件路径进行操作
    if (argc > 1) {
        QString arg(argv[1]);
        wm.switchApkFile(arg);
    }

    return a.exec();
}
