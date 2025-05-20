#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <qfileinfo.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFileInfo file(QString("/proc/%1/exe").arg(4061105));
    qDebug() << file.symLinkTarget();

    return 0;
}
