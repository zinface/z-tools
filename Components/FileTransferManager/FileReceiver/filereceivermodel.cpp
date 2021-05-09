#include "filereceivermodel.h"

#include "../fileiteminfo.h"

#include <QDir>
#include <QTextStream>

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
        case FileItemInfo::FileReceiverNameRole:
            return file->fileName;
        case FileItemInfo::FileReceiverSizeStringRole:
            return file->fileSize;
        case FileItemInfo::FileReceiverSizeRole:
            return file->filesize;
        case FileItemInfo::FileReceiverPathRole:
            return  file->filePath;
        case FileItemInfo::FileReceiverDownloadRole:
            return file->_downstate;
        case FileItemInfo::FileReceiverDownloadSize:
            return file->_totalBytes;
    }

    return QVariant();
}

void FileReceiverModel::appendFile(const QString &filename, qint64 filesize)
{
    FileItemInfo *file = new FileItemInfo;
    file->setName(filename);
    file->setSize(filesize);
    file->setpath(QDir(savePath).absoluteFilePath(filename));
    file->setFileDownloadStat(QFile(file->filePath).exists()?QFile(file->filePath).size() == file->filesize?FileItemInfo::DOWNLOADED:FileItemInfo::SIZEWARRING:FileItemInfo::NOT_DOWNLOAD);

    connect(file, &FileItemInfo::onBytesChanged, this, &FileReceiverModel::downloadByteChanged);
    QTextStream(stdout) << filename << ":" << file->filePath << ":" << (QFile(file->filePath).exists()?"DOWNLOADED":"NOT_DOWNLOAD") << "\n";
    this->files.append(file);
    emit layoutChanged();
}

void FileReceiverModel::appendFile(const QString &filename)
{
    appendFile(filename, 0);
}

void FileReceiverModel::appendFilse(const QStringList &filenames)
{
    emit layoutChanged();
}

void FileReceiverModel::deleteFile(const QString &fileName)
{
    foreach(FileItemInfo *file, this->files) {
        if (file->fileName == fileName) {
            files.removeOne(file);
            delete file;
            break;
        }
    }
    emit layoutChanged();
}

void FileReceiverModel::deleteFiles(const QStringList &fileNames)
{
    foreach(QString filename, fileNames) {
        deleteFile(filename);
    }
}

void FileReceiverModel::clearFile()
{
    this->files.clear();
    emit layoutChanged();
}

void FileReceiverModel::setFileSize(const QString &fileName, qint64 filesize)
{
    for (int i = 0; i < this->files.count(); i++) {
        if (files[i]->fileName == fileName) {
            files[i]->setSize(filesize);
        }
    }
    emit layoutChanged();
}

int FileReceiverModel::count()
{
    return this->files.count();
}

FileItemInfo *FileReceiverModel::item(int i)
{
    return this->files.at(i);
}

void FileReceiverModel::setSavePath(QString &savePath)
{
    this->savePath = savePath;
}

void FileReceiverModel::downloadByteChanged()
{
    emit this->layoutChanged();
}
