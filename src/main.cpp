#include "mainwindow.h"
#include "screenutil.h"

#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QStyleFactory>
#include <version.h>

//#include "adbutils.h"

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

#ifdef QT_DEBUG
    char *apk = getenv("DEBUG_APK_FILE");
    if (apk) {
        QMetaObject::invokeMethod(&wm, "switchApkFile", Q_ARG(QString, apk));
    } else {
        #ifdef DEBUG_APK_FILE
            QMetaObject::invokeMethod(&wm, "switchApkFile", Q_ARG(QString, DEBUG_APK_FILE));
        #endif  // DEBUG_APK_FILE
    }
#else
    // 对可能传入的 apk 文件路径进行操作
    if (argc > 1) {
        QString arg(argv[1]);
        wm.switchApkFile(arg);
    }
#endif

//    auto devices = AdbUtils::GetDevices();
//    qDebug() << devices;
//    if (!devices.isEmpty()) {
//        qInfo() << "Brand:" << AdbUtils::GetBrand(devices.at(0)).trimmed();
//        qInfo() << "Model:" << AdbUtils::GetModel(devices.at(0)).trimmed();
//        qInfo() << "Manufacturer:" << AdbUtils::GetManufacturer(devices.at(0)).trimmed();
//        qInfo() << "AndroidApiLevel:" << AdbUtils::GetAndroidApiLevel(devices.at(0)).trimmed();
//        qInfo() << "AndroidVersion:" << AdbUtils::GetAndroidVersion(devices.at(0)).trimmed();
//        qInfo() << "AndroidWmSize:" << AdbUtils::GetAndroidWmSize(devices.at(0)).trimmed();
//        // AdbUtils::GetAndroidPackages();
//    }

     return a.exec();
}
