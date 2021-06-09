#ifndef FILERECEIVERCOMMAND_H
#define FILERECEIVERCOMMAND_H

#include <QObject>
#include <QTimer>
#include <filetransfermanager.h>
#include <scanworkerpool.h>

class FileReceiverCommand : public QObject
{
    Q_OBJECT
public:
    explicit FileReceiverCommand(QObject *parent = nullptr);
    void setSenderFiles(QStringList &filepaths);
    void setWorkMode(QString mode);
    void setWorkDir(QString path);
    bool setWorkHost(QString host);
    bool setWorkPort(int port);
    void setWorkSpeed(int speed);
    void setFilterFileSuffix(QString suffix);
    bool setIgnoreFileSize(int size);
    void setDownloadFinishExit(bool _exit);
    bool setWorkDownloadThreadNums(int num);

    void showHostFiles();
    void showFiles();

    void scanHost();


    void showHostWork();
    void showWork();

    void searchHostFile();
    void downloadHostFile();

    void start();
    void finish();

private slots:
    void onUnConnected();
    void onConnected();
    void onAppendFile(const QString &fileName, qint64 filesize);
    void onReplyPushFile(QString filename, qint64 filesize);
    void onReplyWork(QString work);

    void scanAvaliableHost(QString host, int port);

    void onScanThreadChanged();
    void onScanFinished();

private:
    QStringList files;
    FileTransferManager manager;
    QString workMode;
    QString workdir;
    QString workhost;
    int workport;
    int workspeed;
    QString worksuffix;
    int workignoresize;
    bool workdone_exit;
    int workthreadnums;
    int cnt;
    int searchCnt;
    int avaliableHostCnt;
    bool onlyShowFile;
    bool onlyShowWork;
    bool onlySearchHostFiles;
    bool onlyDownloadHostFiles;
    qint64 onlyShowFileSize;
    QTimer timer;

    ScanWorkerPool *pool;
};

#endif // FILERECEIVERCOMMAND_H
