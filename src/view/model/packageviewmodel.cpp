#include "packageviewmodel.h"

#include <QDebug>
#include <qapt/package.h>

#include <QTextStream>

PackageViewModel::PackageViewModel(QObject *parent) : QAbstractListModel (parent)
  ,currentArch(ANY_ARCH)
  ,currentCategory(ALL)
  ,filter(FilterPackageName)
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
        case PackageIsInstalledRole:
            return package->isInstalled();
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

/**
 * @brief  模型视图绑定数据更新
 * @note   这个部分有比较复杂的条件与数据过滤的逻辑关系
 * @retval None
 */
void PackageViewModel::updateModel()
{
    m_data.clear();
    QApt::PackageList plist;

    // 此处永远将原有数据集看做旧数据 old_data
    // 将所有符合条件的数据加入到模型用于渲染
    foreach(auto item, old_data) {
        
        // 此处判断为否符合过滤条件，如果不够满足条件，将标记为不包含到模型中用于显示 (conteined = false)
        bool contained = true;
        switch(filter) {
            case FilterPackageName:
                for (auto &sep : currentPackages)
                {
                    if (! QString(item->name()).contains(sep, Qt::CaseInsensitive)) {
                        contained = false;
                        goto exp;
                    }
                }
                break;
            case FilterPackageDescription:
                if (currentPackageDescription.isEmpty()) goto exp;
                for (auto &sep : currentPackageDescriptions)
                {
                    if (! QString(item->longDescription()).contains(sep, Qt::CaseInsensitive)) {
                        contained = false;
                        goto exp;
                    }
                }
                break;
            case FilterPackageSuggestion:
                if (currentPackageSuggestion.isEmpty()) goto exp;
                for (auto &sep : currentPackageSuggests)
                {
                    if (! item->suggestsList().contains(sep, Qt::CaseInsensitive)) {
                        contained = false;
                        goto exp;
                    }
                }
                break;
            case FilterPackageVersion:
                if (currentPackageVersion.isEmpty()) goto exp;
                for (auto &sep : currentPackageVersions)
                {
                    if (! item->version().contains(sep, Qt::CaseInsensitive)) {
                        contained = false;
                        goto exp;
                    }
                }
                break;
        }

        exp:

        if (((filter == FilterPackageName) && currentPackage.isEmpty()) || contained ){
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
    filter = FilterPackageName;
    updateModel();
}

void PackageViewModel::packageDescriptionChange(QString text) {
    currentPackageDescription = text;
    currentPackageDescriptions = currentPackageDescription.split(" ");
    filter = FilterPackageDescription;
    updateModel();
}


void PackageViewModel::packageSuggestionChange(QString text) {
    currentPackageSuggestion = text;
    currentPackageSuggests = currentPackageSuggestion.split(" ");
    filter = FilterPackageSuggestion;
    updateModel();
}

void PackageViewModel::packageVersionChange(QString text) {
    currentPackageVersion = text;
    currentPackageVersions = currentPackageVersion.split(" ");
    filter = FilterPackageVersion;
    updateModel();
}
