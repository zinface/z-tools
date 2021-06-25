#ifndef SEARCHVIEWMODEL_H
#define SEARCHVIEWMODEL_H

#include <QAbstractTableModel>
#include <QApt/Package>

class PackageViewModel : public QAbstractListModel
{
public:
    PackageViewModel(QObject *parent = nullptr);

    enum PackageRole
    {
        PackageNameRole = Qt::DisplayRole,
        UnusedRole = Qt::UserRole,
        WorkerIsPrepareRole,
        ItemIsCurrentRole,
        PackageVersionRole,
        PackagePathRole,
        PackageArchitecture,
        PackageInstalledVersionRole,
        PackageDescriptionRole,
        PackageVersionStatusRole,
        PackageDependsStatusRole,
        PackageAvailableDependsListRole,
        PackageFailReasonRole,
        PackageOperateStatusRole,
        PackageReverseDependsListRole,
    };

    enum PackageInstallStatus
    {
        NotInstalled,
        InstalledSameVersion,
        InstalledEarlierVersion,
        InstalledLaterVersion,
    };

    /***** 架构变量 *****/
    enum PACKAGE_ARCH {
        ANY_ARCH,
        ONLY_I386,
        ONLY_AMD64,
        ONLY_ARM64,
    };

    /***** 安装状态 *****/
    enum PACKAGE_STATE{
        ALL,
        ONLY_INSTALLER,
        ONLY_UNINSTALLER,
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

public slots:
    void setPackages(const QApt::PackageList packages);
    void updateModel();

    void installStatusCategoryChange(int i);
    void packageArchCategoryChange(int i);
    void packageNameChange(QString text);

signals:
    void statusChanged();

private:
    int currentArch;
    int currentCategory;
    QString currentPackage;

private:
    QApt::PackageList m_data;
    QApt::PackageList old_data;

};

#endif // SEARCHVIEWMODEL_H
