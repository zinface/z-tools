#ifndef APTSENDERCOMMANDS_H
#define APTSENDERCOMMANDS_H

#include <QObject>
#include <QTcpSocket>
#include <aptutils.h>
#include <infomationmanager.h>

class AptSenderCommands : public QObject
{
    Q_OBJECT
public:
    explicit AptSenderCommands(QObject *parent = nullptr);
    bool setWorkPort(int port);

    void start();

private slots:
    void onRemoteFetchRaw(qint8 action, qint64 length, QByteArray &data, QTcpSocket *c);
    void onRemoteFetch(int action, QTcpSocket *c);

private:
    InfomationManager manager;
    AptUtils utils;

    int workport;
};

#endif // APTSENDERCOMMANDS_H
