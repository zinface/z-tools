#include "linuxprocutils.h"

#include <QFile>
#include <QTextStream>
#include <QDir>
#include <cstring>
#include <unistd.h>
#include <limits.h>
#include <QRegularExpression>

LinuxProcUtils::LinuxProcUtils(QObject *parent)
    : QObject(parent)
{
}

QList<MountInfo> LinuxProcUtils::parseMounts()
{
    QList<MountInfo> mountList;
    QFile file("/proc/mounts");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return mountList;

    QTextStream stream(&file);
    QString line;
    while (stream.readLineInto(&line))
    {
        QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (parts.size() < 6)
            continue;

        MountInfo info;
        info.source = parts[0];
        info.target = parts[1];
        info.fstype = parts[2];
        info.options = parts[3];
        info.dump = parts[4].toInt();
        info.passno = parts[5].toInt();

        mountList.append(info);
    }
    file.close();
    return mountList;
}

MountInfo LinuxProcUtils::findMountByTarget(const QString &mountTarget)
{
    auto mountList = parseMounts();
    for (const auto& info : mountList)
    {
        if (info.target == mountTarget)
            return info;
    }
    return {};
}

QList<qint64> LinuxProcUtils::getPidsByMountTarget(const QString &mountTarget)
{
    QList<qint64> pidList;
    QDir procDir("/proc");
    auto entryList = procDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QString& entry : entryList)
    {
        bool ok = false;
        qint64 pid = entry.toLongLong(&ok);
        if (!ok)
            continue;

        QString fdPath = QString("/proc/%1/fd").arg(pid);
        QDir fdDir(fdPath);
        auto fdEntries = fdDir.entryList(QDir::Files | QDir::NoDotAndDotDot);

        for (const QString& fd : fdEntries)
        {
            char buf[PATH_MAX] = {0};
            QString fullFd = fdDir.filePath(fd);
            ssize_t len = readlink(fullFd.toLocal8Bit().data(), buf, PATH_MAX - 1);
            if (len <= 0)
                continue;
            buf[len] = '\0';
            QString linkPath = QString::fromLocal8Bit(buf);

            if (linkPath.startsWith(mountTarget))
            {
                pidList.append(pid);
                break;
            }
        }
    }
    return pidList;
}

QString LinuxProcUtils::getProcessExePath(qint64 pid)
{
    char buf[PATH_MAX] = {0};
    QString exeFile = QString("/proc/%1/exe").arg(pid);
    ssize_t len = readlink(exeFile.toLocal8Bit().data(), buf, PATH_MAX - 1);
    if (len <= 0)
        return {};
    buf[len] = '\0';
    return QString::fromLocal8Bit(buf);
}

QString LinuxProcUtils::getProcessCmdlineFirstArg(qint64 pid)
{
    QFile cmdFile(QString("/proc/%1/cmdline").arg(pid));
    if (!cmdFile.open(QIODevice::ReadOnly))
        return {};
    QByteArray data = cmdFile.readAll();
    cmdFile.close();
    return QString::fromLocal8Bit(data.split('\0').first());
}

QString LinuxProcUtils::findAppImagePathFromMountPids(const QList<qint64> &pidList)
{
    for (qint64 pid : pidList)
    {
        QString exePath = getProcessExePath(pid);
        if (exePath.endsWith(".AppImage"))
        {
            return exePath;
        }
        QString cmdPath = getProcessCmdlineFirstArg(pid);
        if (cmdPath.endsWith(".AppImage"))
        {
            return cmdPath;
        }
    }
    return {};
}

QList<qint64> LinuxProcUtils::pgrepFullCmd(const QString &keyword)
{
    QMap<qint64, QString> retMap = pgrepWithCmd(keyword);
    return retMap.keys();
}

QMap<qint64, QString> LinuxProcUtils::pgrepWithCmd(const QString &keyword)
{
    QMap<qint64, QString> result;
    QDir procDir("/proc");
    QStringList dirs = procDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QString &name : dirs)
    {
        bool ok;
        qint64 pid = name.toLongLong(&ok);
        if (!ok) continue;

        QString cmdPath = QString("/proc/%1/cmdline").arg(pid);
        QFile f(cmdPath);
        if (!f.open(QIODevice::ReadOnly)) continue;

        QByteArray raw = f.readAll();
        // \0 分割参数，转正常空格分隔字符串
        QString cmdLine = QString::fromLocal8Bit(raw.replace('\0', ' ')).trimmed();
        if (cmdLine.contains(keyword, Qt::CaseSensitive))
        {
            result.insert(pid, cmdLine);
        }
    }
    return result;
}
