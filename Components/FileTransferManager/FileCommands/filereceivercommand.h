#ifndef FILERECEIVERCOMMAND_H
#define FILERECEIVERCOMMAND_H

#include <QObject>
#include <filetransfermanager.h>

class FileReceiverCommand : public QObject
{
    Q_OBJECT
public:
    explicit FileReceiverCommand(QObject *parent = nullptr);
    void setWorkDir(QString path);
    bool setWorkHost(QString host);
    bool setWorkPort(int port);
    void setWorkSpeed(int speed);
    void setFilterFileSuffix(QString suffix);
    bool setIgnoreFileSize(int size);
    void setDownloadFinishExit(bool _exit);
    bool setWorkDownloadThreadNums(int num);

    void start();

private slots:
    void onUnConnected();
    void onConnected();
    void onAppendFile(const QString &fileName, qint64 filesize);

private:
    QStringList files;
    FileTransferManager manager;
    QString workdir;
    QString workhost;
    int workport;
    int workspeed;
    QString worksuffix;
    int workignoresize;
    bool workdone_exit;
    int workthreadnums;
};

#endif // FILERECEIVERCOMMAND_H
