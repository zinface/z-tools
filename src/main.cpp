
#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QScreen>
#include <aptmanager.h>
#include "config.h"

void moveCenter(QWidget &wm) {
    QScreen *screen = QApplication::screenAt(QCursor::pos());

    auto desktop = screen->geometry();
    
    // 窗口的 x 位置
    auto afterX = (desktop.width() - wm.width())/ 2;
    // 窗口的 y 位置，居中靠上
    auto afterY = (desktop.height() - wm.height()) / 2;
    
    wm.move(afterX + desktop.x(), afterY + desktop.y());
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("软件包管理器 - v" PROJECT_VERSION);
    AptManager wm;
    wm.show();
    
    moveCenter(wm);
    
    return a.exec();
}
