#include "packageinfo.h"

PackageInfo::PackageInfo(QObject *parent) : QObject(parent)
  ,PackageName(QString())
  ,PackageVersion(QString())
  ,PackageArchitecture(QString())
  ,PackageInstalled(false)
{

}

void PackageInfo::setName(const QString &Name)
{
    this->PackageName = Name;
}

void PackageInfo::setVersion(const QString &Version)
{
    this->PackageVersion = Version;
}

void PackageInfo::setArchitecture(const QString &Architecture)
{
    this->PackageArchitecture = Architecture;
}

void PackageInfo::setInstalled(const bool Installed)
{
    this->PackageInstalled = Installed;
}
