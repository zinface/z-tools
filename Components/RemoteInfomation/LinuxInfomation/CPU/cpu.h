#ifndef CPU_H
#define CPU_H

#include <QObject>

#define PROC_CPU "/proc/stat"

class Cpu : public QObject
{
    Q_OBJECT
public:
    explicit Cpu(QObject *parent = nullptr);

    QString getCpuUsed();

signals:


private:
    QString path;
    long old_cpuAll = 0;
    long old_cpuFree = 0;
};

#endif // CPU_H
