#include "packageviewmodel.h"

#include <QDebug>

PackageViewModel::PackageViewModel(QObject *parent) : QAbstractListModel (parent)
  ,currentArch(ANY_ARCH)
  ,currentCategory(ALL)
{
//    connect(this, &PackageViewModel::statusChanged, this, setPackages())
}

int PackageViewModel::rowCount(const QModelIndex &parent) const
{
    return m_data.count();
}

QVariant PackageViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int nrow = index.row();
    PackageInfo *package = m_data.at(nrow);

    switch (role)
    {
        case PackageNameRole:
            return package->getName();
        case PackageVersionRole:
            return package->getVersion();
        case PackageInstalledVersionRole:
            return package->getInstalled() ? InstalledSameVersion : NotInstalled;
        case PackageArchitecture:
            return package->getArchitecture();
        default:;
    }

    return QVariant();
}

void PackageViewModel::appendPackage(PackageInfo *package)
{
    old_data.append(package);
    updateModel();
}

void PackageViewModel::setPackages(const QList<PackageInfo *> packages)
{
    old_data = packages;
    updateModel();
}

void PackageViewModel::updateModel()
{
    m_data.clear();
    QList<PackageInfo*> plist;

    foreach(auto item, old_data) {
        if (currentPackage.isEmpty() || QString(item->getName()).contains(currentPackage)){
            switch (currentCategory) {
            case ALL:
                plist.append(item);
                break;
            case ONLY_INSTALLER:
                if (item->getInstalled()) {
                    plist.append(item);
                };break;
            case ONLY_UNINSTALLER:
                if (!item->getInstalled()) {
                    plist.append(item);
                };break;
            }
        }
    }

    foreach(auto item, plist) {
        switch (currentArch) {
        case ANY_ARCH:
            m_data.append(item);
            break;
        case ONLY_I386:
            if (item->getArchitecture().compare("i386") == 0) {
                m_data.append(item);
            };break;
        case ONLY_AMD64:
            if (item->getArchitecture().compare("amd64") == 0) {
                m_data.append(item);
            };break;
        case ONLY_ARM64:
            if (item->getArchitecture().compare("arm64") == 0) {
                m_data.append(item);
            };break;
        }
    }

    emit layoutChanged();
}

void PackageViewModel::installStatusCategoryChange(int i)
{
    currentCategory = i;
    updateModel();
}

void PackageViewModel::packageArchCategoryChange(int i)
{
    currentArch = i;
    updateModel();
}

void PackageViewModel::packageNameChange(QString text)
{
    currentPackage = text;
    updateModel();
}
