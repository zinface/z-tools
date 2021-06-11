#include "uptime.h"
#include <QFile>
#include <QTextStream>
#include <QtMath>

Uptime::Uptime(QObject *parent) : QObject(parent)
  ,path("/proc/uptime")
{

}

QString Uptime::getUptime()
{
    int uptime;
    QFile file("/proc/uptime"); // /proc/uptime
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return QString("(fail)");
    QTextStream stream(&file);
    uptime = qFloor(stream.readLine().split(QRegExp("\\s{1,}")).at(0).toDouble());
//    uptime = qFloor(QString("96557.08 718567.54").split(QRegExp("\\s{1,}")).at(0).toDouble());
    int s = 60;
    int m = 60*60;
    int h = 60*60*24;
    int d = 60*60*24;
    int ss =  uptime % s;
    int mm = (uptime % m) / s;
    int hh = (uptime % h) / m;
    int dd = (uptime / d);
    QString runTime = QString("系统已运行: %1天, %2:%3:%4")
            .arg(dd, 0, 'f', 0, QLatin1Char(' '))
            .arg(hh, 2, 'f', 0, QLatin1Char('0'))
            .arg(mm, 2, 'f', 0, QLatin1Char('0'))
            .arg(ss, 2, 'f', 0, QLatin1Char('0'));


    return runTime;
}
