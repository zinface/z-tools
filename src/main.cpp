#include "mainwindow.h"
#include "screenutil.h"

#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QStyleFactory>
#include <version.h>

int main(int argc, char *argv[])
{
#ifdef Q_OS_MACOS
    qApp->setStyle(QStyleFactory::create("Fusion"));
#endif
    QApplication a(argc, argv);
    a.setApplicationName("base-apk-viewer");
    a.setApplicationDisplayName("apk文件查看器 - v" PROJECT_VERSION "(" BUILD_DATE ")");
    MainWindow wm;
    wm.show();
    
    ScreenUtil::moveCenter(wm);

    // 对可能传入的 apk 文件路径进行操作
    if (argc > 1) {
        QString arg(argv[1]);
        wm.switchApkFile(arg);
    }

    return a.exec();
}
