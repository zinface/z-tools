#include "filereceiveritemdelegate.h"
#include "filereceivermodel.h"
#include "filereceiverview.h"

FileReceiverView::FileReceiverView(QWidget *parent) : QListView(parent)
  ,mfileReceiverItemDelegate(new FileReceiverItemDelegate)
  ,mfileReciverModel(new FileReceiverModel)
  ,currentRow(-1)
{
    this->setItemDelegate(mfileReceiverItemDelegate);
    this->setModel(mfileReciverModel);

    connect(this, &QListView::clicked, this, &FileReceiverView::onclicked);
}

void FileReceiverView::appendFile(const QString &fileName, qint64 filesize)
{
    this->mfileReciverModel->appendFile(fileName, filesize);
}

void FileReceiverView::appendFile(const QString &fileName)
{
    this->mfileReciverModel->appendFile(fileName);
}

void FileReceiverView::appendFilse(const QStringList &files)
{
    this->mfileReciverModel->appendFilse(files);
}

void FileReceiverView::onclicked(const QModelIndex &index)
{
    this->currentRow = index.row();
}

FileReceiverModel *FileReceiverView::getMode() {
    return mfileReciverModel;
}
