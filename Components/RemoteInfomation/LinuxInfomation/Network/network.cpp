#include "network.h"

#include <QFile>
#include <QTextStream>

Network::Network(QObject *parent) : QObject(parent)
{
}

long old_upload;
QString Network::getNetUpload()
{
    long uploadAll = 0;

    long upload = 0;

    QFile file("/proc/net/dev");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return QString("(fail)");
    QTextStream stream(&file);
    QString line = stream.readLine();
    line  = stream.readLine();
    line  = stream.readLine();
    while (!line.isNull()) {
        line = line.trimmed();
        QStringList list = line.split(QRegExp("\\s{1,}"));   // 匹配任意 大于等于1个的 空白字符

        if (!list.isEmpty()) {
            upload = list.at(9).toLong();
        }

        uploadAll += upload;
        line  = stream.readLine();
    }
    file.close();
    double upspeed = 0;
    SpeedRateUnit upunit = SpeedRateByte;
    QString uploadUnit = autoRateUnits(uploadAll - old_upload, upunit, upspeed);

    old_upload = uploadAll;
    return QString("↑:%1").arg(upspeed, 0, 'f', 2, QLatin1Char(' ')) + uploadUnit;
}

long old_download;
QString Network::getNetDownload()
{
    long downloadAll = 0;

    long download = 0;

    QFile file("/proc/net/dev");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return QString("(fail)");
    QTextStream stream(&file);
    QString line = stream.readLine();
    line  = stream.readLine();
    line  = stream.readLine();
    while (!line.isNull()) {
        line = line.trimmed();
        QStringList list = line.split(QRegExp("\\s{1,}"));   // 匹配任意 大于等于1个的 空白字符

        if (!list.isEmpty()) {
            download = list.at(1).toLong();
        }

        downloadAll += download;
        line  = stream.readLine();
    }
    file.close();
    double dospeed = 0;
    SpeedRateUnit dounit = SpeedRateByte;
    QString downUnit = autoRateUnits(downloadAll - old_download, dounit, dospeed);

    old_download = downloadAll;

    return QString("↓:%1").arg(dospeed, 0, 'f', 2, QLatin1Char(' ')) + downUnit;
}

QString Network::convertRateUnits(Network::SpeedRateUnit &unit)
{
    switch (unit) {
    case SpeedRateBit:
        return QString("bit/s");
    case SpeedRateByte:
        return QString("b/s");
    case SpeedRateKb:
        return QString("kb/s");
    case SpeedRateMb:
        return QString("mb/s");
    case SpeedRateGb:
        return QString("gb/s");
    case SpeedRateTb:
        return QString("tb/s");
    default:
        return QString("");
    }
}

QString Network::autoRateUnits(long speed, Network::SpeedRateUnit &unit, double &sp)
{
    sp = 0;
    if        (speed >= 0 && speed < qPow(2,10)) { // qPow(2,10) = 1024b
        unit = SpeedRateByte;
        sp = speed;
    } else if (speed >= qPow(2,10) && speed < qPow(2,20)) { // qPow(2,20) = 1024k
        unit = SpeedRateKb;
        sp =  (speed /  qPow(2,10) * 1.0);
    } else if (speed >= qPow(2,20) && speed < qPow(2,30)) { // qPow(2,30) = 1024m
        unit = SpeedRateMb;
        sp =  (speed /  qPow(2,20) * 1.0);
    } else if (speed >= qPow(2,30) && speed < qPow(2,40)) { // qPow(2,40) = 1024g
        unit = SpeedRateGb;
        sp =  (speed /  qPow(2,30) * 1.0);
    } else if (speed >= qPow(2,40) && speed < qPow(2,50)) { // qPow(2,50) = 1024t
        unit = SpeedRateTb;
        sp =  (speed /  qPow(2,40) * 1.0);
    } else {                                                // qPow(2,60) = 1024z => ?
        unit = SpeedRateUnknow;
        sp =  -1;
    }
    speed = sp;
    return convertRateUnits(unit);
}
