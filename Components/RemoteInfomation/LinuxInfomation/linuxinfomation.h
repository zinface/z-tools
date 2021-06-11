#ifndef LINUXINFOMATION_H
#define LINUXINFOMATION_H

#include <QObject>
#include <QTimer>
#include <cpu.h>
#include <memory.h>
#include <network.h>
#include <uptime.h>

class LinuxInfomation : public QObject
{
    Q_OBJECT
public:
    explicit LinuxInfomation(QObject *parent = nullptr);

    enum INFO_TYPE {
        CPU,
        MEM,
        NETUP,
        NETDOWN,
        UPTIME,
    };

    QString getCPU();
    QString getMemory();
    QString getNetUpload();
    QString getNetDownload();
    QString getUptime();

    void netstart();

    QString currentNetUp;
    QString currentNetDown;

private slots:
    void UpdateNetwork();

private:
    Cpu cpu;
    Memory memory;
    Network network;
    Uptime uptime;

    QTimer timer;
};

#endif // LINUXINFOMATION_H
