#include "filereceivercommand.h"

#include <QFileInfo>
#include <QNetworkInterface>
#include <QRegExpValidator>
#include <scanworkerpool.h>
#include <scanworkerthread.h>

FileReceiverCommand::FileReceiverCommand(QObject *parent) : QObject(parent)
  ,workMode("Download")
  ,workhost()
  ,workport(8888)
  ,workspeed(-1)
  ,worksuffix("")
  ,workignoresize(-1)
  ,workdone_exit(false)
  ,workthreadnums(1)
  ,cnt(0)
  ,searchCnt(0)
  ,avaliableHostCnt(0)
  ,onlyShowFile(false)
  ,onlyShowWork(false)
  ,onlyShowWorkTasks(false)
  ,onlySearchHostFiles(false)
  ,onlyDownloadHostFiles(false)
  ,onlyShowFileSize(0)
{
    this->manager.setMaxTaskToggetherRunningCount(this->workthreadnums);
    connect(&manager, &FileTransferManager::connected, this, &FileReceiverCommand::onConnected);
    connect(&manager, &FileTransferManager::onRemoteFileAppend, this, &FileReceiverCommand::onAppendFile);
    connect(&manager, &FileTransferManager::ClientSocketUnConnected, this, &FileReceiverCommand::onUnConnected);
    connect(&manager, &FileTransferManager::onReplyPushFileConfirm, this, &FileReceiverCommand::onReplyPushFile);
    connect(&manager, &FileTransferManager::onReplyFetchWork, this, &FileReceiverCommand::onReplyWork);
    connect(&manager, &FileTransferManager::onReplyFetchWorkTasks, this, &FileReceiverCommand::onReplyWorkTasks);
}

void FileReceiverCommand::setSenderFiles(QStringList &filepaths)
{
    this->files.append(filepaths);
}

void FileReceiverCommand::setWorkMode(QString mode)
{
    this->workMode = mode;
}

void FileReceiverCommand::setWorkDir(QString path)
{
    this->workdir = path;
    manager.setSavePath(this->workdir);
}

bool FileReceiverCommand::setWorkHost(QString host)
{
    QRegExp rxIp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    QRegExpValidator v(rxIp);
    int pos = 0;
    if (v.validate(host,pos) != QRegExpValidator::Acceptable) goto _failhost;
    this->workhost = host;
    return true;

_failhost:
    this->workhost = host;
    return false;
}

bool FileReceiverCommand::setWorkPort(int port)
{
    this->workport = port;
    return true;
}

void FileReceiverCommand::setWorkSpeed(int speed)
{
    this->workspeed = speed;
}

void FileReceiverCommand::setFilterFileSuffix(QString suffix)
{
    this->worksuffix = suffix;
}

bool FileReceiverCommand::setIgnoreFileSize(int size)
{
    if (size > 0) {
        this->workignoresize = size;
    }
    return true;
}

void FileReceiverCommand::setDownloadFinishExit(bool _exit)
{
    this->workdone_exit = _exit;
}

bool FileReceiverCommand::setWorkDownloadThreadNums(int num)
{
    this->workthreadnums = num;
    if (num <= 0 || num > 64) goto _failthreadnums;
    this->manager.setMaxTaskToggetherRunningCount(this->workthreadnums);
    return true;

    _failthreadnums:
    return false;
}

void FileReceiverCommand::showHostFiles()
{
    this->onlyShowFile = true;
    timer.setInterval(2000);
    connect(&timer, &QTimer::timeout, this, &FileReceiverCommand::showFiles);
    timer.start();
    start();
}

void FileReceiverCommand::showFiles()
{
    if (cnt == 0) goto timeout;
    QTextStream(stdout) << QString("好家伙, 云盘有 %1 个文件，共计 %2 kb.\n").arg(cnt).arg(onlyShowFileSize/1024);
    exit(0);

    timeout:
    QTextStream(stdout) << QString("好家伙, 超时未响应.\n");
    exit(-1);
}

void FileReceiverCommand::scanHost()
{
    QTextStream(stdout) << QString("好家伙, 准备扫描主机.\n");
    pool = new ScanWorkerPool;
    connect(pool, &QThread::finished, this, &FileReceiverCommand::onScanFinished);
    connect(pool, &ScanWorkerPool::onTaskThreadChanged, this, &FileReceiverCommand::onScanThreadChanged);

    QRegExp rxIp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    QRegExpValidator v(rxIp);
    QList<QHostAddress> localAllAddresses = QNetworkInterface::allAddresses();
    foreach(QHostAddress address, localAllAddresses){
        QString ipAddr = address.toString();
        int pos = 0;
        // use the first IP address
        if (ipAddr == "127.0.0.1") continue;
        if (v.validate(ipAddr,pos) == QRegExpValidator::Acceptable){
            QStringList localSplit = ipAddr.split(".");
            QString prefixIp = QString("%1.%2.%3").arg(localSplit[0]).arg(localSplit[1]).arg(localSplit[2]);
            for(int i = 1; i < 255; i++) {
                if (ipAddr == QString("%1.%2").arg(prefixIp).arg(i)) continue;
                ScanWorkerThread *localScanWorkerThread = new ScanWorkerThread(QString("%1.%2").arg(prefixIp).arg(i), workport);
                connect(localScanWorkerThread, &ScanWorkerThread::onConnected, this, &FileReceiverCommand::scanAvaliableHost);
                pool->addThreadTask(localScanWorkerThread);
            }
        }
    }
    pool->setMaxTaskTogether(50);
    pool->setWaitForFinished(true);
    pool->doStart();
}

void FileReceiverCommand::showHostWork()
{
    QTextStream(stdout) << QString("好家伙, 准备获取主机工作目录.\n");
    this->onlyShowWork = true;
    timer.setInterval(1000);
    connect(&timer, &QTimer::timeout, this, &FileReceiverCommand::finish);
    timer.start();
    start();
}

void FileReceiverCommand::showHostWorkTasks()
{
    QTextStream(stdout) << QString("好家伙, 准备获取主机工作任务数据.\n");
    this->onlyShowWorkTasks = true;
    timer.setInterval(1000);
    connect(&timer, &QTimer::timeout, this, &FileReceiverCommand::finish);
    timer.start();
    start();
}

void FileReceiverCommand::searchHostFile()
{
    QTextStream(stdout) << QString("好家伙, 准备搜索主机匹配文件.\n");
    this->onlySearchHostFiles = true;
    timer.setInterval(2000);
    connect(&timer, &QTimer::timeout, this, &FileReceiverCommand::finish);
    timer.start();
    start();
}

void FileReceiverCommand::downloadHostFile()
{
    QTextStream(stdout) << QString("好家伙, 准备拉取指定主机文件.\n");
    this->onlyDownloadHostFiles = true;
    timer.setInterval(2000);
    connect(&timer, &QTimer::timeout, this, &FileReceiverCommand::finish);
    timer.start();
    start();
}

void FileReceiverCommand::start()
{
    QTextStream(stdout) << QString("好家伙, 准备连接：%1: %2\n").arg(this->workhost).arg(this->workport);
    manager.setManagerTask(this->workhost, this->workport, SessionManager::CLIENT);
}

// 工作完成
void FileReceiverCommand::finish()
{
    // 计数器检查
    if (cnt == 0) goto timeout;
    // 仅下载 -
    if(onlyDownloadHostFiles) return;
    // 极速触发 - 匹配相似文件完成检查
    if (onlySearchHostFiles) {
        QTextStream(stdout) << QString("好家伙, 己匹配主机文件数量 %1 个.\n").arg(searchCnt);
    }
    exit(0);

    timeout:
    QTextStream(stdout) << QString("好家伙, 超时未响应.\n");
    exit(-1);
}

void FileReceiverCommand::onUnConnected()
{
    QTextStream(stdout) << QString("好家伙, 连接失败(退出).\n");
    exit(0);
}

void FileReceiverCommand::onConnected()
{
//    QTextStream(stdout) << QString("好家伙, 连接成功 - 模式%1\n").arg(this->workMode);
    // 极速触发 --work 显示Cloud目录
    if (onlyShowWork) {
        manager.fetchWorkAction();
        return;
    }
    // 极速触发 --tasks 查看服务端当前运行任务数
    if (onlyShowWorkTasks) {
        manager.fetchWorkTasksAction();
        return;
    }
    // 极速触发 --target <name> 下载指定文件
    if (onlyDownloadHostFiles) {
        manager.fetchFileListAction();
        return;
    }
    // 触发 - 常规下载
    if (workMode == "Download")
        manager.fetchFileListAction();
    // 触发 - 上传模式 - 文件上传确认
    else if (workMode == "Upload") {
        foreach(QString filepath, this->files) {
            QFileInfo info(filepath);
            manager.fetchPushFileConfirm(info.fileName(), info.size());
        }
    }
}

void FileReceiverCommand::onAppendFile(const QString &fileName, qint64 filesize)
{
    cnt++;
    // 极速触发 --list 列出文件列表
    if (onlyShowFile) {
        timer.stop();
        onlyShowFileSize += filesize;
        QTextStream(stdout) << QString("好家伙, 确认中: %1\r").arg(cnt%3==0?QString("\\"):cnt%3==1?QString("/"):QString("-"));
        timer.start();
        return;
    }
    // 极速触发 --search <name> 匹配相似文件列表
    if (onlySearchHostFiles) {
        QTextStream(stdout) << QString("好家伙, 确认中: %1\r").arg(cnt%3==0?QString("\\"):cnt%3==1?QString("/"):QString("-"));
        timer.stop();
        foreach(QString file, this->files) {
            if (fileName.contains(file)) {
                searchCnt++;
                QTextStream(stdout) << QString("好家伙, 发现相似: %1\n").arg(fileName);
            }
        }
        timer.start();
        return;
    }
    // 极速触发 --target <name> 下载指定文件
    if (onlyDownloadHostFiles) {
        QTextStream(stdout) << QString("好家伙, 确认中: %1\r").arg(cnt%3==0?QString("\\"):cnt%3==1?QString("/"):QString("-"));
        timer.stop();
        foreach(QString file, this->files) {
            if (fileName == file) {
                QTextStream(stdout) << QString("好家伙, 确认文件: %1\n").arg(fileName);
                manager.fetchFileAction(nullptr, fileName, filesize, this->workdir);
            }
        }
        timer.start();
        return;
    }
    // 触发拦截 -- 上传/列出文件列表/查看任务情况
    if (this->workMode == "Upload" || onlyShowWork || onlyShowWorkTasks) return;
    // 触发拦截 -- 名称尾部不匹配
    if (!this->worksuffix.isEmpty() && !fileName.endsWith(this->worksuffix)) goto _notsuffix;
    // 触发拦截 -- 文件大小不符合
    if (this->workignoresize<filesize && this->workignoresize != -1) goto _ignore_filesize;

    QTextStream(stdout) << QString("好家伙, 确认中: %1 - %2\r").arg(cnt%3==0?QString("\\"):cnt%3==1?QString("/"):QString("-")).arg(fileName);
    // 文件传输管理器下载任务 - 创建下载
    manager.fetchFileAction(nullptr, fileName, filesize, this->workdir);

    _ignore_filesize:;
    _notsuffix:;
}

void FileReceiverCommand::onReplyPushFile(QString filename, qint64 filesize)
{
    // 本地文件列表
    foreach(QString filepath, this->files) {
        QFileInfo info(filepath);
        if (info.fileName() == filename) {
            cnt++;
            QTextStream(stdout) << QString("好家伙, 确认中: %1 - %2\r").arg(cnt%3==0?QString("\\"):cnt%3==1?QString("/"):QString("-")).arg(filename);
            manager.broadCaseAction(nullptr, FileTransferManager::OP_UPLOAD, info.fileName(), info.size(), filepath);
        }
    }
}

void FileReceiverCommand::onReplyWork(QString work)
{
    cnt++;
    QTextStream(stdout) << QString("好家伙, 云盘当前工作目录: %1.\n").arg(work);
}

void FileReceiverCommand::onReplyWorkTasks(QString workTasks)
{
    cnt++;
    QTextStream(stdout) << QString("好家伙, 主机当前工作任务数为: %1.\n").arg(workTasks);
}

void FileReceiverCommand::scanAvaliableHost(QString host, int port)
{
    avaliableHostCnt++;
    QTextStream(stdout) << QString("好家伙, 发现主机: %1:%2 \n").arg(host).arg(port);
}

void FileReceiverCommand::onScanThreadChanged()
{
    QTextStream(stdout) << QString("好家伙, 扫描中: %1 \r").arg(pool->count()%3==0?QString("\\"):pool->count()%3==1?QString("/"):QString("-"));
}

void FileReceiverCommand::onScanFinished()
{
    QTextStream(stdout) << QString("好家伙, 找到有 %1 个主机.\n").arg(avaliableHostCnt);
    exit(0);
}
