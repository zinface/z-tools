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

class AptUtils : public QObject
{
    Q_OBJECT
public:
    explicit AptUtils(QObject *parent = nullptr);


public:
    // 已经安装数量
    int GetInstalledPackagesCount();
    // 软件源数数量
    int GetMirrorsPackagesCount();

    // 所有软件包
    QApt::PackageList GetPackageList();

private:
    QApt::PackageList plist;
    QApt::Backend m_backend;
};

#endif // APTUTILS_H
