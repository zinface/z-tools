#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QtMath>

#define PROC_NET "/proc/net/dev"

class Network : public QObject
{
    Q_OBJECT
public:
    explicit Network(QObject *parent = nullptr);

    enum SpeedRateUnit {
        SpeedRateBit,
        SpeedRateByte,
        SpeedRateKb,
        SpeedRateMb,
        SpeedRateGb,
        SpeedRateTb,
        SpeedRateUnknow
    };

    QString getNetUpload();
    QString getNetDownload();

signals:


private:
    QString convertRateUnits(SpeedRateUnit &unit);
    QString autoRateUnits(long speed, SpeedRateUnit &unit, double &sp);

private:
    QString path;
};

#endif // NETWORK_H
