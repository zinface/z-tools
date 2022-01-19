#include "packageviewmodel.h"

#include <QDebug>
#include <qapt/package.h>

#include <QTextStream>

PackageViewModel::PackageViewModel(QObject *parent) : QAbstractListModel (parent)
  ,currentArch(ANY_ARCH)
  ,currentCategory(ALL)
{
//    connect(this, &PackageViewModel::statusChanged, this, setPackages())
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
            return package->isInstalled() ? (Installed && package->state() & QApt::Package::State::Upgradeable) ? Upgradeable : Installed : NotInstalled;
        case PackageArchitecture:
            return package->architecture();
        default:;
    }

    return QVariant();
}

QApt::Package* PackageViewModel::package(const int index) {
    return m_data.at(index);
}

void PackageViewModel::setPackages(const QApt::PackageList packages)
{
    old_data = packages;
    updateModel();
}

void PackageViewModel::updateModel()
{
    m_data.clear();
    QApt::PackageList plist;

    foreach(auto item, old_data) {
        
        bool contained = true;
        for (auto &sep : currentPackages)
        {
            if (! item->name().contains(sep)) {
                contained = false;
                goto exp;
            }
        }

        exp:

        if (currentPackage.isEmpty() || contained ){
            switch (currentCategory) {
            case ALL:
                plist.append(item);
                break;
            case ONLY_INSTALLER:
                if (item->isInstalled()) {
                    plist.append(item);
                };break;
            case ONLY_UNINSTALLER:
                if (!item->isInstalled()) {
                    plist.append(item);
                };break;
            case ONLY_UPGRADLEABLE:
                if (item->state() & QApt::Package::State::Upgradeable) {
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
            if (item->architecture().compare("i386") == 0) {
                m_data.append(item);
            };break;
        case ONLY_AMD64:
            if (item->architecture().compare("amd64") == 0) {
                m_data.append(item);
            };break;
        case ONLY_ARM64:
            if (item->architecture().compare("arm64") == 0) {
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
    currentPackages = currentPackage.split(" ");
    updateModel();
}
