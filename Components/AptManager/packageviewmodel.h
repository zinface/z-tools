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

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

public slots:

    void setPackages(const QApt::PackageList packages);

private:
    QApt::PackageList m_data;

};

#endif // SEARCHVIEWMODEL_H
