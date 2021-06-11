#ifndef INFORECIVERCOMMANDS_H
#define INFORECIVERCOMMANDS_H

#include <QObject>
#include <infomationmanager.h>

class InfoReciverCommands : public QObject
{
    Q_OBJECT
public:
    explicit InfoReciverCommands(QObject *parent = nullptr);
    void setWorkMode(QString mode);
    bool setWorkHost(QString host);
    bool setWorkPort(int port);

    void start();
    void finish();

private slots:
    void onConnected();
    void onUnConnected();

    void onReplay(int action, QString msg);

private:
    InfomationManager manager;

    QString _work;
    QString _host;
    int _port;

};

#endif // INFORECIVERCOMMANDS_H
