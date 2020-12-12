#include "adbpkgview.h"

AdbPkgView::AdbPkgView(QWidget *parent) : QListView(parent)
//  ,m_data(new QStringList)
  ,m_model(new QStringListModel)
{
    setModel(m_model);
}

void AdbPkgView::appendData(const QString &item)
{
    m_data.append(item);
    m_model->setStringList(m_data);
}

void AdbPkgView::setData(QStringList items)
{
    m_data = items;
    m_model->setStringList(m_data);
}
