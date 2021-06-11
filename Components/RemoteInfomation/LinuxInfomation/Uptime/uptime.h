#ifndef UPTIME_H
#define UPTIME_H

#include <QObject>

class Uptime : public QObject
{
    Q_OBJECT
public:
    explicit Uptime(QObject *parent = nullptr);

    QString getUptime();

signals:


private:
    QString path;
};

#endif // UPTIME_H
