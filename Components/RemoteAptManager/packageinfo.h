#ifndef PACKAGEINFO_H
#define PACKAGEINFO_H

#include <QObject>

class PackageInfo : public QObject
{
    Q_OBJECT
public:
    explicit PackageInfo(QObject *parent = nullptr);
    void setName(const QString &Name);
    void setVersion(const QString &Version);
    void setArchitecture(const QString &Architecture);
    void setInstalled(const bool Installed);

    QString getName() {return this->PackageName;}
    QString getVersion() {return this->PackageVersion;}
    QString getArchitecture() {return this->PackageArchitecture;}
    bool getInstalled() {return this->PackageInstalled;}

private:
    QString PackageName;
    QString PackageVersion;
    QString PackageArchitecture;
    bool PackageInstalled;

};

#endif // PACKAGEINFO_H
