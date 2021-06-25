#include "aptsendercommands.h"

#include <QNetworkInterface>
#include <remoteaptmanager.h>

AptSenderCommands::AptSenderCommands(QObject *parent) : QObject(parent)
  ,workport(8888)
{
    connect(&manager, &InfomationManager::onRemoteFetch, this, &AptSenderCommands::onRemoteFetch);
    connect(&manager, &InfomationManager::onRemoteFetchRaw, this, &AptSenderCommands::onRemoteFetchRaw);
    manager.registerAction(RemoteAptManager::FetchPackageList, true);
    manager.registerAction(RemoteAptManager::RemoteLocalPackageCount);
    manager.registerAction(RemoteAptManager::RemoteMirrorsPackageCount);
}

bool AptSenderCommands::setWorkPort(int port)
{
    this->workport = port;
    return true;
}

void AptSenderCommands::start()
{
    this->manager.setManagerTask("", this->workport, SessionManager::SERVER);
    foreach(QHostAddress address, QNetworkInterface::allAddresses()){
        QString ipAddr = address.toString();
        QTextStream(stdout) << QString(" -- 等待连接中: %1: %2\n").arg(ipAddr).arg(this->workport);
    }
}

void AptSenderCommands::onRemoteFetchRaw(qint8 action, qint64 length, QByteArray &data, QTcpSocket *c)
{
    if (action == RemoteAptManager::FetchPackageList) {
        QStringList packages;
        foreach(auto item, utils.GetPackageList()) {
            QStringList package;
            package << item->name() << item->version() << item->architecture() << (item->isInstalled() ? QString("true"):QString("false"));
            packages << package.join(":");
        }
        QByteArray data = packages.join(";").toLocal8Bit();
        manager.broadCaseAction(c, action, packages.join(";").toLocal8Bit().length(), data);
        QTextStream(stdout) << QString("%1\n").arg(data.length());
    }
}

void AptSenderCommands::onRemoteFetch(int action, QTcpSocket *c)
{
    if (action == RemoteAptManager::FetchPackageList) {
        QStringList packages;
        foreach(auto item, utils.GetPackageList()) {
            packages << item->name();
        }
        QByteArray data = packages.join(";").toLocal8Bit();
        manager.broadCaseAction(c, action, packages.join(";").toLocal8Bit().length(), data);
    } else if (action == RemoteAptManager::Name) {

    } else if (action == RemoteAptManager::Version) {

    } else if (action == RemoteAptManager::Arch) {

    } else if (action == RemoteAptManager::Installed) {

    } else if (action == RemoteAptManager::RemoteLocalPackageCount) {
        QTextStream(stdout) << QString("RemoteLocalPackageCount\n");
        manager.broadCaseAction(c, action, QString("%1").arg(utils.GetInstalledPackagesCount()));
    } else if (action == RemoteAptManager::RemoteMirrorsPackageCount) {
        QTextStream(stdout) << QString("RemoteMirrorsPackageCount\n");
        manager.broadCaseAction(c, action, QString("%1").arg(utils.GetMirrorsPackagesCount()));
    }
}
