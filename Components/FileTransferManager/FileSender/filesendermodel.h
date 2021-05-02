#ifndef FILESENDERMODEL_H
#define FILESENDERMODEL_H

#include <QAbstractListModel>
#include <QFile>


class FileItemInfo;

class FileSenderModel : public QAbstractListModel
{
    Q_OBJECT
public:
    FileSenderModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

public:
    void appendFile(const QString &filePath);
    void appendFilse(const QStringList &filePaths);

    void filesDeleted(QString filepath);
    void filesCleanded();

    //
    int count();
    FileItemInfo* item(int i);

private:
    QFile mfile;
    QList<FileItemInfo*> files;
};

#endif // FILESENDERMODEL_H
