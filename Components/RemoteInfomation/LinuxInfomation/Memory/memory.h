#ifndef MEMORY_H
#define MEMORY_H

#include <QObject>

#define PROC_MEM "/proc/meminfo"

class Memory : public QObject
{
    Q_OBJECT
public:
    explicit Memory(QObject *parent = nullptr);
    QString getMemUsed();

signals:

private:
    QString path;
};

#endif // MEMORY_H
