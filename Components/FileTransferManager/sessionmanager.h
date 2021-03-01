#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QMap>
#include <QObject>
#include <QTcpSocket>

class SessionManager : public QObject
{
    Q_OBJECT
public:
    explicit SessionManager(QObject *parent = nullptr);

signals:

public slots:

private:
    QMap<QString, QTcpSocket*> smap;
};

#endif // SESSIONMANAGER_H
