#ifndef APTUTILS_H
#define APTUTILS_H

#include <QApt/Backend>


//namespace QApt {
//    class Cache;
//    class Config;
//    class DebFile;
//    class Transaction;

//    class Backend;
//    class Package;
//}

class AptUtils
{

public:
    static int GetInstalledPackagesCount(const QApt::Backend *backend);
    static int GetMirrorsPackagesCount(const QApt::Backend *backend);

    static QApt::PackageList GetPackageList(const QApt::Backend *backend);
    static QApt::PackageList GetPackageListArchitecture(const QApt::Backend *backend, const QString& text);
    static QApt::PackageList GetPackageListContians(const QApt::Backend *backend, const QString& text);

//    static QApt::PackageList GetPackageListContians(const QApt::Backend *backend, const QString& text);

};

#endif // APTUTILS_H
