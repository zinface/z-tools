#include "memory.h"

#include <QFile>
#include <QTextStream>

Memory::Memory(QObject *parent) : QObject(parent)
  ,path(PROC_MEM)
{

}

QString Memory::getMemUsed()
{
    long memory;
    long memoryAll;
    long swap;
    long swapAll;

    QFile file("/proc/meminfo");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return QString("(fail)");
    QTextStream stream(&file);
    long buff[16] = {0};
    for (int i = 0; i <= 15; ++i) {
        QString line = stream.readLine();
        QStringList list = line.split(QRegExp("\\s{1,}"));
        buff[i] = list.at(1).toLong();
    }
    file.close();

    memoryAll = buff[0];
    memory = buff[0] - buff[2];
    swapAll = buff[14];
    swap = buff[14] - buff[15];

//    if (memory * 100.0 / memoryAll < 50)
//        mMemUsed->setStyleSheet("QLabel{ color:rgb(79, 158, 57)}");
//    if (memory * 100.0 / memoryAll > 50)
//        mMemUsed->setStyleSheet("QLabel{ color:rgb(199, 189, 80)}");
//    if (memory * 100.0 / memoryAll > 80)
//        mMemUsed->setStyleSheet("QLabel{ color:rgb(208, 50, 47)}");
    return QString("MEM:%1%").arg(memory * 100.0 / memoryAll, 0, 'f', 2, QLatin1Char(' '));
}
