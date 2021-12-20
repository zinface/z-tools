#ifndef DIRECTORYRECEIVER_H
#define DIRECTORYRECEIVER_H

#include <QObject>
#include <filetransfermanager.h>

class DirectoryReceiver : public QObject
{
    Q_OBJECT
public:
    explicit DirectoryReceiver(QObject *parent = nullptr);
    void setWorkDir(QString path) {this->_workDir = path;}
    void setDepth(int depth);
    void start();

private:
    void handleWorkDir(qint8 ftype, QString path);

private slots:
    void onConnected();

    void onReplyFetchDirectoryTree(qint8 ftype, qint64 filesize, QString path);


private:
    FileTransferManager manager;
    int _depth;
    QString _workDir;
    QString _workHost;

};

#endif // DIRECTORYRECEIVER_H
