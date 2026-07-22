#ifndef LINUXPROCUTILS_H
#define LINUXPROCUTILS_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>

// 挂载项结构体，对应/proc/mounts一行6字段
struct MountInfo
{
    QString source;         // 挂载源
    QString target;         // 挂载点
    QString fstype;         // 文件系统类型
    QString options;        // 挂载参数
    int dump;               // dump标记
    int passno;             // fsck检查序号
};

// LM-Studio-0.4.10-1-x64.AppImage
// /tmp/.mount_LM-StuM5qWE5
// fuse.LM-Studio-0.4.10-1-x64.AppImage
// ro,nosuid,nodev,relatime,user_id=1000,group_id=1000
// 0
// 0

class LinuxProcUtils : public QObject
{
    Q_OBJECT
public:
    explicit LinuxProcUtils(QObject *parent = nullptr);

    // 1. 解析 /proc/mounts 返回所有挂载列表
    static QList<MountInfo> parseMounts();

    // 2. 根据挂载点精确查找挂载信息
    static MountInfo findMountByTarget(const QString& mountTarget);

    // 3. 获取占用指定挂载点的所有PID（等价fuser）
    static QList<qint64> getPidsByMountTarget(const QString& mountTarget);

    // 4. 根据PID读取 /proc/[pid]/exe 软链接，返回可执行绝对路径
    static QString getProcessExePath(qint64 pid);

    // 5. 根据PID读取 /proc/[pid]/cmdline 获取启动命令第一个参数
    static QString getProcessCmdlineFirstArg(qint64 pid);

    // 6. 从挂载点关联PID列表中，自动筛选出原始AppImage路径
    static QString findAppImagePathFromMountPids(const QList<qint64>& pidList);


    // 根据命令行关键字模糊匹配，等价 pgrep -f
    static QList<qint64> pgrepFullCmd(const QString &keyword);
    // 等价 pgrep -af：返回 <pid, cmdline> 键值对
    static QMap<qint64, QString> pgrepWithCmd(const QString &keyword);
};

#endif // LINUXPROCUTILS_H
