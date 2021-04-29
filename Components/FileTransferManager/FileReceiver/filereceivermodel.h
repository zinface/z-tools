#ifndef FILERECIEVERMODEL_H
#define FILERECIEVERMODEL_H

#include <QAbstractListModel>
#include <QFile>

class FileItemInfo;

class FileReceiverModel : public QAbstractListModel
{
    Q_OBJECT
public:
    FileReceiverModel(QObject *parent = 0);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

public:
    void appendFile(const QString &fileName, qint64 filesize);
    void appendFile(const QString &fileName);
    void appendFilse(const QStringList &files);

    int count();

private:
    QFile mfile;
    QList<FileItemInfo*> files;
};

#endif // FILERECIEVERMODEL_H
