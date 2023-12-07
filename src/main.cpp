#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QStyleFactory>
#include <QScreen>

void moveCenter(QWidget &wm) {
    QScreen *screen = QApplication::screenAt(QCursor::pos());


    auto desktop = screen->geometry();
    
    auto afterX = (desktop.width() - wm.width())/ 2;
    auto afterY = (desktop.height() - wm.height()) /2;
    
    wm.move(afterX + desktop.x(), afterY + desktop.y());
}

int main(int argc, char *argv[])
{
#ifdef Q_OS_MACOS
    qApp->setStyle(QStyleFactory::create("Fusion"));
#endif
    QApplication a(argc, argv);
    a.setApplicationName("apk文件查看器 - v1.0");
    MainWindow wm;
    wm.show();
    
    moveCenter(wm);

    // 对可能传入的 apk 文件路径进行操作
    if (argc > 1) {
        QString arg(argv[1]);
        wm.switchApkFile(arg);
    }

    return a.exec();
}
