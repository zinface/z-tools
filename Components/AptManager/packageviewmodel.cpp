#include "packageviewmodel.h"

#include <QDebug>

PackageViewModel::PackageViewModel(QObject *parent) : QAbstractListModel (parent)
{
}

int PackageViewModel::rowCount(const QModelIndex &parent) const
{
    return m_data.size();
}

QVariant PackageViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int nrow = index.row();
    QApt::Package *package = m_data.at(nrow);

    switch (role)
    {
        case PackageNameRole:
            return package->name();
        case PackageVersionRole:
            return package->version();
        case PackageInstalledVersionRole:
            return package->isInstalled() ? InstalledSameVersion : NotInstalled;
        case PackageArchitecture:
            return package->architecture();
        default:;
    }

    return QVariant();
}



void PackageViewModel::setPackages(const QApt::PackageList packages)
{
    m_data.clear();
    m_data = packages;
    emit layoutChanged();
}
