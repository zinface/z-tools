#include "filereceiveritemdelegate.h"
#include "filereceivermodel.h"
#include "filereceiverview.h"

FileReceiverView::FileReceiverView(QWidget *parent) : QListView(parent)
  ,currentRow(-1)
  ,mfileReceiverItemDelegate(new FileReceiverItemDelegate)
  ,mfileReciverModel(new FileReceiverModel)
{
    this->setItemDelegate(mfileReceiverItemDelegate);
    this->setModel(mfileReciverModel);

    connect(this, &QListView::clicked, this, &FileReceiverView::onclicked);
    connect(this->mfileReceiverItemDelegate, &FileReceiverItemDelegate::downloadItem, this, &FileReceiverView::onDownloadFile);
}

void FileReceiverView::appendFile(const QString &fileName, qint64 filesize)
{
    this->mfileReciverModel->appendFile(fileName, filesize);
}

void FileReceiverView::deleteFile(const QString &fileName)
{
    this->mfileReciverModel->deleteFile(fileName);
}

void FileReceiverView::clearFile()
{
    this->mfileReciverModel->clearFile();
}

void FileReceiverView::setFileSize(const QString &fileName, qint64 filesize)
{
    this->mfileReciverModel->setFileSize(fileName, filesize);
}

void FileReceiverView::onclicked(const QModelIndex &index)
{
    this->currentRow = index.row();
}

FileReceiverModel *FileReceiverView::getMode() {
    return mfileReciverModel;
}

void FileReceiverView::setSavePath(QString &savePath)
{
    this->mfileReciverModel->setSavePath(savePath);
}

FileItemInfo *FileReceiverView::item(int i)
{
    return this->mfileReciverModel->item(i);
}

void FileReceiverView::onDownloadFile(const QModelIndex index)
{
    QString filename = index.data(FileItemInfo::FileReceiverNameRole).toString();
    if (this->item(index.row())->_downstate == FileItemInfo::DOWNLOADED || this->item(index.row())->_downstate == FileItemInfo::DOWNLOADING) {
        return;
    }
    this->item(index.row())->setFileDownloadStat(FileItemInfo::DOWNLOADING);
    emit this->downloadFile(filename);
    emit this->downloadFileItemInfo(*this->item(index.row()));
    emit this->mfileReciverModel->layoutChanged();
}
