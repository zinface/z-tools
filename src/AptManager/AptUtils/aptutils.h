#ifndef APTUTILS_H
#define APTUTILS_H

#include <QApt/Backend>

class AptUtils : public QObject
{
    Q_OBJECT
public:
    explicit AptUtils(QObject *parent = nullptr);
    ~AptUtils();

public:
    // 已经安装数量
    int GetInstalledPackagesCount();
    // 可更新数量
    int GetUpgradablePackagesCount();
    // 软件源可用数量
    int GetMirrorsPackagesCount();

    void reload();

    // 所有软件包
    QApt::PackageList GetPackageList();

signals:
    void packageChange();

private:
    QApt::Backend m_backend;
};

#endif // APTUTILS_H
