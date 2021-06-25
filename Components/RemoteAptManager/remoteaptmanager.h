#ifndef REMOTEAPTMANAGER_H
#define REMOTEAPTMANAGER_H

#include <QObject>

class RemoteAptManager : public QObject
{
    Q_OBJECT
public:
    explicit RemoteAptManager(QObject *parent = nullptr);

    enum RemotePackageAction {
        FetchPackageList,
        Name,
        Version,
        Arch,
        Installed,
        RemoteLocalPackageCount,
        RemoteMirrorsPackageCount,
    };

//    QList<

signals:

};

#endif // REMOTEAPTMANAGER_H
