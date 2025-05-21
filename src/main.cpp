#include "mainwindow.h"

#include <QApplication>
#include <QMainWindow>
#include <bamfdialog.h>

#include <screenutil.h>
#include <version.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("z-tools-desktop-generater");
    a.setApplicationDisplayName("桌面图标生成器 - v" PROJECT_VERSION);

    qRegisterMetaType<Application>("Application");

    MainWindow wm;
    wm.show();
    
    ScreenUtil::moveCenterForCursor(wm);

    return a.exec();
}
