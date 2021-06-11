#include "cpu.h"

#include <QFile>
#include <QTextStream>

Cpu::Cpu(QObject *parent) : QObject(parent)
  ,path(PROC_CPU)
{

}

long old_cpuAll = 0;
long old_cpuFree = 0;

QString Cpu::getCpuUsed()
{
    long cpuAll = 0;
    long cpuFree = 0;
    QFile file(path); // /proc/stat
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return QString("(fail)");
    QTextStream stream(&file);
    QStringList list = stream.readLine().split(QRegExp("\\s{1,}"));
    foreach (QString str, list){
        cpuAll += str.toLong();
    }
    cpuFree = list.at(4).toLong();

//    if (((cpuAll - old_cpuAll) - (cpuFree - old_cpuFree)) * 100.0 / (cpuAll - old_cpuAll) < 50)
//        mCpuUsed->setStyleSheet("QLabel{ color:rgb(79, 158, 57)}");
//    if (((cpuAll - old_cpuAll) - (cpuFree - old_cpuFree)) * 100.0 / (cpuAll - old_cpuAll) > 50)
//        mCpuUsed->setStyleSheet("QLabel{ color:rgb(199, 189, 80)}");
//    if (((cpuAll - old_cpuAll) - (cpuFree - old_cpuFree)) * 100.0 / (cpuAll - old_cpuAll) > 80)
//        mCpuUsed->setStyleSheet("QLabel{ color:rgb(208, 50, 47)}");
    QString result = QString("CPU:%1%").arg((((cpuAll - old_cpuAll) - (cpuFree - old_cpuFree)) * 100.0 / (cpuAll - old_cpuAll)), 0, 'f', 2, QLatin1Char(' '));
    old_cpuAll = cpuAll;
    old_cpuFree = cpuFree;

    return result;
}
