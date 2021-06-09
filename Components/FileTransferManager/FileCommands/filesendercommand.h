#ifndef FILESENDERCOMMAND_H
#define FILESENDERCOMMAND_H

#include <QObject>
#include <filetransfermanager.h>

class FileSenderCommand : public QObject
{
    Q_OBJECT
public:
    explicit FileSenderCommand(QObject *parent = nullptr);
    void setSenderFiles(QStringList &filepaths);
    void setWorkMode(QString mode);
    void setWorkDir(QString path);
    bool setWorkPort(int port);
    bool setWorkThreadNums(int num = 64);

    void start();

private slots:
    void onClientFetchFileList(QTcpSocket *c);
    void onClientFetchFile(QTcpSocket *c, QString filename);
    void onClientPushFileConfirm(QTcpSocket *c, QString filename, qint64 filesize);
    void onClientPushFile(QTcpSocket *c, QString filename, qint64 filesize);
    void onClientFetchWork(QTcpSocket *c);

    void onServerUnListenError();

private:
    QStringList files;
    FileTransferManager manager;
    QString workMode;
    QString workdir;
    int workport;
    int workthreadnums;
};

#endif // FILESENDERCOMMAND_H
