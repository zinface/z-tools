#include <QApplication>

#include "apkiconshowmainwindow.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    QApplication a(argc, argv);

    a.setApplicationDisplayName("apk-icon-show");

    ApkIconShowMainWindow w;
    w.show();

    return a.exec();
}
