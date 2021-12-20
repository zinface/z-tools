#ifndef DIRECTORYSENDER_H
#define DIRECTORYSENDER_H

#include <QDir>
#include <QObject>
#include <QTcpSocket>
#include <infomationmanager.h>

class DirectorySender : public QObject
{
    Q_OBJECT
public:
    explicit DirectorySender(QObject *parent = nullptr);
    void setWorkDir(QString path) {this->_workDir = path;}
    void start();

private:
    void broadCaseFileInfo();
    void broadCaseWorkDirectoryTree(qint8 action, QTcpSocket *c, QString parent, QString relative);

signals:

private slots:
    // Subscriber
    void onRemoteFetchRaw(qint8 action, qint64 length, QByteArray &data, QTcpSocket *c);
    void onRemoteFetch(qint8 action, QTcpSocket *c);

private:
    InfomationManager manager;

    // 文件目录服务者工作目录
    QString _workDir;
    QString _workHost;
    QString _workPort;

};

#endif // DIRECTORYSENDER_H
