#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QMainWindow>

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
    QApplication a(argc, argv);
    a.setApplicationName("桌面图标生成器 - v1.2.1");
    MainWindow wm;
    wm.show();
    
    moveCenter(wm);

    return a.exec();
}
