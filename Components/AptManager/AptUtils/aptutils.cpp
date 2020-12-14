#include "aptutils.h"

#include <QDebug>
#include <QSysInfo>
#include <QTextStream>

int AptUtils::GetInstalledPackagesCount(const QApt::Backend *backend)
{
    return backend->installedCount();
}

int AptUtils::GetMirrorsPackagesCount(const QApt::Backend *backend)
{
    return backend->packageCount();

}

QApt::PackageList AptUtils::GetPackageList(const QApt::Backend *backend)
{
    return GetPackageListArchitecture(backend, "amd64");
}

QApt::PackageList AptUtils::GetPackageListArchitecture(const QApt::Backend *backend, const QString &text)
{
    QApt::PackageList plist;
    foreach(const QApt::Package *package,  backend->availablePackages()) {
//        if (!package->architecture().compare("amd64"))
            plist.append((QApt::Package*)package);
    }
    return plist;
}

QApt::PackageList AptUtils::GetPackageListContians(const QApt::Backend *backend, const QString &text)
{
    QTextStream cout(stdout);

    QApt::PackageList plist;
    foreach(const QApt::Package *package,  GetPackageList(backend)) {
        if (QString(package->name()).contains(text)) {
            plist.append((QApt::Package*)package);
        }
    }
    return plist;
}


