#include "ScreenUtils.h"
#include <QApplication>
#include <QWindow>
#include <QDebug>

ScreenUtils::ScreenUtils(QObject *parent) : QObject(parent) {
    
}

void ScreenUtils::moveScreenCenter(QWidget &window, QScreen *screen) {
    QRect screenRect = screen->geometry();

    //  获取屏幕的位置信息，计算出相对位置
    int x = screenRect.x();
    int y = screenRect.y();
    int width = screenRect.width();
    int height = screenRect.height();

    int wm_x = (width - window.width()) / 2 ;
    int wm_y = (height - window.height()) / 2 ;

    window.move(x + wm_x, y + wm_y);
}

void ScreenUtils::moveScreenTopRight(QWidget &window, QScreen *screen) {
    QRect screenRect = screen->geometry();

    //  获取屏幕的位置信息，计算出相对位置
    int x = screenRect.x();
    int y = screenRect.y();
    int width = screenRect.width();
    int height = screenRect.height();

    int wm_x = (width - window.width());
    int wm_y = 0;

    window.move(x + wm_x, y + wm_y);
}


void ScreenUtils::movePrimaryScreenCenter(QWidget &window) {
    QScreen *screen = QGuiApplication::primaryScreen();  // 主屏幕
    moveScreenCenter(window, screen);
}

void ScreenUtils::moveCenter(QWidget &window) {
    QDesktopWidget *desktop = QApplication::desktop();
    if (desktop->screenCount() == 1)
        // window.move((desktop->width() - window.width()) /2,(desktop->height() - window.height()) /2);
        movePrimaryScreenCenter(window);
    else {
        moveScreenCenter(window, window.windowHandle()->screen());
    }
}

void ScreenUtils::moveCenterForCursor(QWidget &window) {
    int cx = QCursor::pos().x();
    int cy = QCursor::pos().y();
    
    // 对鼠标进行一次屏幕轮询识别在哪个屏幕上
    for (QScreen *screen : QGuiApplication::screens()) {
        int x = screen->geometry().x();
        int y = screen->geometry().y();
        int width = screen->geometry().width();
        int height = screen->geometry().height();

        // 鼠标范围检查规则，在屏幕坐标原点第四象限范围
        // 1. 向下不超过屏幕所在位置的高度
        // 2. 向右不超过屏幕所在位置的宽度
        if ((cy > y && cy < y + height) && (cx > x && cx < x + width)) {
            moveScreenCenter(window, screen);
        }
    }
}

void ScreenUtils::moveMouseCenter(QWidget &wm) {
    QScreen *screen = QApplication::screenAt(QCursor::pos());
    auto desktop = screen->geometry();
    auto afterX = (desktop.width() - wm.width()) / 2;
    auto afterY = (desktop.height() - wm.height()) /2;

    wm.move(afterX + desktop.x(), afterY + desktop.y());

    qDebug() << screen->availableGeometry();
    qDebug() << screen->geometry();
    qDebug() << QCursor::pos();
}
