#ifndef SEARCHVIEW_H
#define SEARCHVIEW_H

#include <QListView>
#include <QTableView>

#include <QApt/Package>

class PackageViewModel;
class PackagesListDelegate;

class PackageView : public QListView
{
    Q_OBJECT
public:
    explicit PackageView(QWidget *parent = nullptr);

signals:
    void appendPackage(const QApt::Package &package);
    void setPackages(const QApt::PackageList &packages);

    void setInstallCategory(int i);
    void setArchCategory(int i);
    void setPackageName(QString text);
    void setPackageDescription(QString text);
    void setPackageSuggestion(QString text);
    void setPackageVersion(QString text);

    void currentPackageChanged(QApt::Package *package);

private:
    PackageViewModel *m_model;
    PackagesListDelegate *m_packagesListDelegate;
};

#endif // SEARCHVIEW_H
