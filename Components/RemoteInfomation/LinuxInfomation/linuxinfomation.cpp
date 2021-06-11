#include "linuxinfomation.h"

LinuxInfomation::LinuxInfomation(QObject *parent) : QObject(parent)
{
    timer.setInterval(1000);
    connect(&timer, &QTimer::timeout, this, &LinuxInfomation::UpdateNetwork);
}

QString LinuxInfomation::getCPU()
{
    return cpu.getCpuUsed();
}

QString LinuxInfomation::getMemory()
{
    return memory.getMemUsed();
}

QString LinuxInfomation::getNetUpload()
{
    return currentNetUp;
}

QString LinuxInfomation::getNetDownload()
{
    return currentNetDown;
}

QString LinuxInfomation::getUptime()
{
    return uptime.getUptime();
}

void LinuxInfomation::netstart()
{
    timer.start();
    UpdateNetwork();
}

void LinuxInfomation::UpdateNetwork()
{
    currentNetUp = network.getNetUpload();
    currentNetDown = network.getNetDownload();
}
