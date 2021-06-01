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
    bool setWorkPort(int port);
    bool setWorkThreadNums(int num = 64);

    void start();

private slots:
    void onClientFetchFileList(QTcpSocket *c);
    void onClientFetchFile(QTcpSocket *c, QString filename);

private:
    QStringList files;
    FileTransferManager manager;
    int workport;
    int workthreadnums;
};

#endif // FILESENDERCOMMAND_H
