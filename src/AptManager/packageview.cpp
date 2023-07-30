#include "packageslistdelegate.h"
#include "packageview.h"
#include "packageviewmodel.h"

#include <QHeaderView>
#include <qobjectdefs.h>

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

    connect(this,&PackageView::setPackages, [=](const QApt::PackageList &packages){
      m_model->setPackages(packages);
    });
    connect(this,&PackageView::setInstallCategory, [=](int i){
      m_model->installStatusCategoryChange(i);
    });
    connect(this,&PackageView::setArchCategory, [=](int i){
      m_model->packageArchCategoryChange(i);
    });
    connect(this,&PackageView::setPackageName, [=](QString text){
      m_model->packageNameChange(text);
    });
    connect(this,&PackageView::setPackageDescription,[=](QString text){
      m_model->packageDescriptionChange(text);
    });
    connect(this,&PackageView::setPackageSuggestion, [=](QString text){
      m_model->packageSuggestionChange(text);
    });
    connect(this,&PackageView::setPackageVersion, [=](QString text){
      m_model->packageVersionChange(text);
    });
    connect(m_packagesListDelegate, &PackagesListDelegate::currentPackageIndexChanged, [=](int index){
      // m_model->data(index);
      emit currentPackageChanged(m_model->package(index));
    });
}
