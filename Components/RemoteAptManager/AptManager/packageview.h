#ifndef SEARCHVIEW_H
#define SEARCHVIEW_H

#include <QListView>
#include <QTableView>
#include <packageinfo.h>

class PackageViewModel;
class PackagesListDelegate;

class PackageView : public QListView
{
    Q_OBJECT
public:
    explicit PackageView(QWidget *parent = nullptr);

signals:
    void appendPackage(PackageInfo *package);
    void setPackages(const QList<PackageInfo*> packages);

    void setInstallCategory(int i);
    void setArchCategory(int i);
    void setPackageName(QString text);

private:
    PackageViewModel *m_model;
    PackagesListDelegate *m_packagesListDelegate;
};

#endif // SEARCHVIEW_H
