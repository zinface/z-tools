#include "fileiteminfo.h"
#include <QtMath>

FileItemInfo::FileItemInfo(QObject *parent) : QObject(parent)
{

}

void FileItemInfo::setName(const QString &name)
{
    this->fileName = name;
}

void FileItemInfo::setpath(const QString &path)
{
    this->filePath = path;
}

void FileItemInfo::setSize(qint64 size)
{
    RateUnit unit;
    double val = 0;
    QString rate = autoRateUnits(size, unit, val);
    this->fileSize = QString::number(val, 'f', 2) + rate;
    this->filesize = size;
}

void FileItemInfo::setFileUploadStat(int state)
{
    this->_upstate = FileUploadStat(state);
}

void FileItemInfo::setFileDownloadStat(int state)
{
    this->_downstate = FileDonwloadStat(state);
}

QString FileItemInfo::autoRateUnits(qint64 size, FileItemInfo::RateUnit &unit, double &val)
{
    val = 0;
    if (size >= 0 && size < qPow(2,10)) {                 // qPow(2,10) = 1024b
        unit = RateByte;
        val = size;
    } else if (size >= qPow(2,10) && size < qPow(2,20)) { // qPow(2,20) = 1024k
        unit = RateKb;
        val =  (size /  qPow(2,10) * 1.0);
    } else if (size >= qPow(2,20) && size < qPow(2,30)) { // qPow(2,30) = 1024m
        unit = RateMb;
        val =  (size /  qPow(2,20) * 1.0);
    } else if (size >= qPow(2,30) && size < qPow(2,40)) { // qPow(2,40) = 1024g
        unit = RateGb;
        val =  (size /  qPow(2,30) * 1.0);
    } else if (size >= qPow(2,40) && size < qPow(2,50)) { // qPow(2,50) = 1024t
        unit = RateTb;
        val =  (size /  qPow(2,40) * 1.0);
    } else {                                                // qPow(2,60) = 1024z => ?
        unit = RateUnknow;
        val =  -1;
    }
    size = val;
    return convertRateUnits(unit);
}

QString FileItemInfo::convertRateUnits(FileItemInfo::RateUnit &unit)
{
    switch (unit) {
    case RateBit:
        return QString("Bit");
    case RateByte:
        return QString("B");
    case RateKb:
        return QString("Kb");
    case RateMb:
        return QString("Mb");
    case RateGb:
        return QString("Gb");
    case RateTb:
        return QString("Tb");
    default:
        return QString("");
    }
}


