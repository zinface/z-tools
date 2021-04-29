#include "filereceivermodel.h"
#include "../fileiteminfo.h"

FileReceiverModel::FileReceiverModel(QObject *parent)

{

}

int FileReceiverModel::rowCount(const QModelIndex &parent) const
{
    return this->files.size();
}

QVariant FileReceiverModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return  QVariant();

    const int r = index.row();
    const FileItemInfo *file = this->files.at(r);
    switch (role) {
        case FileItemInfo::FileSenderNameRole:
            return file->fileName;
        case FileItemInfo::FileSenderSizeRole:
            return file->fileSize;
        case FileItemInfo::FileSenderPathRole:
            return  file->filePath;
        case FileItemInfo::FileSenderUploadRole:
            return file->_upstate;
    }

    return QVariant();
}

void FileReceiverModel::appendFile(const QString &fileName, qint64 filesize)
{
    FileItemInfo *file = new FileItemInfo;
    file->setName(fileName);
    file->setSize(filesize);
    file->setFileDownloadStat(FileItemInfo::NOT_DOWNLOAD);
    this->files.append(file);
    emit layoutChanged();
}

void FileReceiverModel::appendFile(const QString &fileName)
{
    FileItemInfo *file = new FileItemInfo;
    file->setName(fileName);
    file->setFileDownloadStat(FileItemInfo::NOT_DOWNLOAD);
    this->files.append(file);
    emit layoutChanged();
}

void FileReceiverModel::appendFilse(const QStringList &files)
{
    emit layoutChanged();
}

int FileReceiverModel::count()
{
    return this->files.count();
}
