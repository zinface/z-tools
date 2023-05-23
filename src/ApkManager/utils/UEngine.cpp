#include "UEngine.h"

#include <QFileInfo>
#include <QProcess>
#include <QThread>
#include <QTime>

UEngine::UEngine(QObject *parent) : QObject(parent)
  ,cmd("uengine")
{
    connect(this, SIGNAL(logChanged(QString)), this, SLOT(storelog(QString)));
    connect(&process, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &UEngine::onFinished);
    connect(&process2, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &UEngine::onFinished2);
}

UEngine::~UEngine()
{
    
}

bool UEngine::checkCommandUEngine()
{
    emit logChanged(QString("1.检查是否已安装 UEngine.\n"));
    QProcess process;
    process.start(QString("which"), QStringList() << "uengine");
    process.waitForFinished();
    return process.exitCode() == 0;
}

bool UEngine::checkCommandReady()
{
//    QProcess process;
    process2.start("uengine", QStringList() << "wait-ready");
//    process.waitForFinished();
//    process.execute("uengine wait-ready");
//    int localExitCode = process.exitCode();
//    QProcess::ExitStatus localExitStatus = process.exitStatus();
//    emit onFinished(localExitCode, localExitStatus);
//    return localExitCode == 0 && localExitStatus == QProcess::ExitStatus::NormalExit;
}

bool UEngine::checkApkFile(QString apkFile)
{
    QFileInfo fileInfo(apkFile);
    if (fileInfo.exists() && fileInfo.isFile()) {
        emit logChanged(QString("有效\n"));
        return true;
    } else {
        emit logChanged(QString("无效.\n"));
        return false;
    }
}

int UEngine::doSyncInstall(QString apkPath)
{
    return doInstall(Sync, apkPath);
}

void UEngine::doAsyncInstall(QString apkPath)
{
    doInstall(Async, apkPath);
}

int UEngine::doInstall(UEngine::InstallType type, QString apkPath)
{
    logs.clear();
    QStringList cmdArgs = QStringList() << "install"
                            << QString("--apk=%1").arg(apkPath);
// 1. check apk file
    emit logChanged(QString("1.检查是APK文件是否有效: "));
    if (!checkApkFile(apkPath)) {
        return -1;
    }

    emit logChanged(QString("2.检查 Android 系统是否成功启动.\n"));
    checkCommandReady();
    return -1;

//// 2. check android os ready
//    emit logChanged(QString("2.检查 Android 系统是否成功启动.\n"));
//    if (!checkCommandReady()) {
//        return -1;
//    }

// 3. do install(async/sync)
    emit logChanged(QString("3.执行安装命令:\n"));
    emit logChanged(QString(" -> %1 %2\n").arg(cmd).arg(cmdArgs.join(" ")));
    if (type == Async) {
        process.start(cmd, cmdArgs);
        emit logChanged("4.安装结果: \n");
        return 0;
    }

// sync
    QProcess process;
    process.start(cmd, cmdArgs);
    emit logChanged("4.安装结果: \n");
    emit logChanged(process.exitCode() == 0? " -> 安装完成.": " -> 安装失败.");
    return process.exitCode();
}

// async
void UEngine::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::ExitStatus::NormalExit) {
        emit logChanged(exitCode==0? " -> 安装完成.": " -> 安装失败.");
    } else {
        emit logChanged(QString(" -> 安装失败，进程崩溃(%1)").arg(exitCode));
    }
    emit finished(exitCode);
}

void UEngine::onFinished2(int exitCode, QProcess::ExitStatus exitStatus)
{
    emit logChanged(exitCode == 0? " -> 已启动.": " -> 未启动.");
//    emit logChanged(QString("2.检查 Android 系统是否成功启动.\n"));
    emit onFinished(exitCode, exitStatus);
}

void UEngine::storelog(QString log)
{
    logs << log;
}
