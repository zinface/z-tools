#include "aptutils.h"

#include <QDebug>
#include <QSysInfo>
#include <QTextStream>
#include <qapt/backend.h>
#include <qapt/package.h>

#include <QThread>
#include <QTimer>

AptUtils::AptUtils(QObject *parent)
{
    Q_UNUSED(parent);
    m_backend.init();
}

AptUtils::~AptUtils() {
}

int AptUtils::GetInstalledPackagesCount()
{
    return m_backend.packageCount(QApt::Package::Installed);
}
int AptUtils::GetUpgradablePackagesCount() {
    return m_backend.packageCount(QApt::Package::Upgradeable);
}
int AptUtils::GetMirrorsPackagesCount()
{
    return m_backend.packageCount();
}

QApt::PackageList AptUtils::GetPackageList()
{
    return m_backend.availablePackages();
}

void AptUtils::reload(){
    m_backend.reloadCache();
}