#include "filereceivercommand.h"

#include <QRegExpValidator>

FileReceiverCommand::FileReceiverCommand(QObject *parent) : QObject(parent)
  ,workhost()
  ,workport(8888)
  ,workspeed(-1)
  ,worksuffix("")
  ,workignoresize(-1)
  ,workdone_exit(false)
  ,workthreadnums(1)
{
    this->manager.setMaxTaskToggetherRunningCount(this->workthreadnums);
    connect(&manager, &FileTransferManager::connected, this, &FileReceiverCommand::onConnected);
    connect(&manager, &FileTransferManager::onRemoteFileAppend, this, &FileReceiverCommand::onAppendFile);
    connect(&manager, &FileTransferManager::ClientSocketUnConnected, this, &FileReceiverCommand::onUnConnected);
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

void FileReceiverCommand::start()
{
    QTextStream(stdout) << QString("好家伙,准备连接：%1， %2\n").arg(this->workhost).arg(this->workport);
    manager.setManagerTask(this->workhost, this->workport, SessionManager::CLIENT);
}

void FileReceiverCommand::onUnConnected()
{
    QTextStream(stdout) << QString("好家伙,连接失败(退出).\n");
    exit(0);
}

void FileReceiverCommand::onConnected()
{
    manager.fetchFileListAction();
    QTextStream(stdout) << QString("好家伙,连接成功\n");
}
int cnt=0;
void FileReceiverCommand::onAppendFile(const QString &fileName, qint64 filesize)
{
    cnt++;
    if (!this->worksuffix.isEmpty() && !fileName.endsWith(this->worksuffix)) goto _notsuffix;
    if (this->workignoresize<filesize && this->workignoresize != -1) goto _ignore_filesize;

    QTextStream(stdout) << QString("好家伙,准备下载：%1 - %2 - %3\n").arg(cnt).arg(fileName).arg(filesize);
    manager.fetchFileAction(fileName, filesize, this->workdir);


    _ignore_filesize:;
    _notsuffix:;
}
