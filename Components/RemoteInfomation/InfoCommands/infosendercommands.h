#ifndef INFOSENDERCOMMANDS_H
#define INFOSENDERCOMMANDS_H

#include <QObject>
#include <QTcpSocket>
#include <infomationmanager.h>
#include <linuxinfomation.h>

class InfoSenderCommands : public QObject
{
    Q_OBJECT
public:
    explicit InfoSenderCommands(QObject *parent = nullptr);
    bool setWorkPort(int port);

    void start();

private slots:
    void onRemoteFetch(int action, QTcpSocket *c);

private:
    InfomationManager manager;
    LinuxInfomation info;

    int _port;
};

#endif // INFOSENDERCOMMANDS_H
