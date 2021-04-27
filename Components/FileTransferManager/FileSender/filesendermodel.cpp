#include "filesendermodel.h"
#include "../fileiteminfo.h"

#include <QFileInfo>
#include <QTextStream>

FileSenderModel::FileSenderModel(QObject *parent)
{

}

int FileSenderModel::rowCount(const QModelIndex &parent) const
{
    return this->files.size();
}

QVariant FileSenderModel::data(const QModelIndex &index, int role) const
{
//    Q_UNUSED(index);
    Q_UNUSED(role);
    if (!index.isValid()) return QVariant();

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

void FileSenderModel::appendFile(const QString &filePath)
{
    Q_UNUSED(filePath);
}

void FileSenderModel::appendFilse(const QStringList &filePaths)
{
    foreach(QString path, filePaths) {
        QFileInfo info(path);
        FileItemInfo *f = new FileItemInfo;
        f->setName(info.fileName());
        f->setpath(info.filePath());
        f->setSize(info.size());
        f->setFileUploadStat(FileItemInfo::FileUploadStat::NOT_UPLOAD);
        this->files.append(f);
    }
    emit layoutChanged();
}

void FileSenderModel::filesDeleted(QString file)
{
    foreach(FileItemInfo *info, files) {
        if (info->filePath == file) {
            files.removeOne(info);
            delete info;
            QTextStream(stdout) << QString("删除 ") << file << QString(" 成功!\n");
            return;
        }
    }
     QTextStream(stdout) << QString("删除 ") << file << QString(" 失败!\n");
}

void FileSenderModel::filesCleanded()
{
    this->files.clear();
}

int FileSenderModel::count()
{
    return this->files.count();
}

FileItemInfo *FileSenderModel::item(int i)
{
    return this->files.at(i);
}
