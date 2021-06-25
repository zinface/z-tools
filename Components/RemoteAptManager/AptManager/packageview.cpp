#include "packageslistdelegate.h"
#include "packageview.h"
#include "packageviewmodel.h"

#include <QHeaderView>

PackageView::PackageView(QWidget *parent) : QListView(parent)
  ,m_model(new PackageViewModel)
  ,m_packagesListDelegate(new PackagesListDelegate)
{
    setModel(m_model);
    setItemDelegate(m_packagesListDelegate);

    setVerticalScrollMode(ScrollPerItem);
//    setSelectionMode(NoSelection);
//    setAutoScroll(false);
//    setMouseTracking(true);

    connect(this,&PackageView::appendPackage, m_model, &PackageViewModel::appendPackage);
    connect(this,&PackageView::setPackages, m_model, &PackageViewModel::setPackages);
    connect(this,&PackageView::setInstallCategory, m_model, &PackageViewModel::installStatusCategoryChange);
    connect(this,&PackageView::setArchCategory, m_model, &PackageViewModel::packageArchCategoryChange);
    connect(this,&PackageView::setPackageName, m_model, &PackageViewModel::packageNameChange);
}
