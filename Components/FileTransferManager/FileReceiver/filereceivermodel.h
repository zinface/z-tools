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
    // file names
    void appendFile(const QString &filename, qint64 filesize);
    void appendFile(const QString &filename);
    void appendFilse(const QStringList &filenames);

    // file names
    void deleteFile(const QString &fileName);
    void deleteFiles(const QStringList &fileNames);

    void clearFile();

    void setFileSize(const QString &fileName, qint64 filesize);

    int count();


    void setSavePath(QString &savePath);
private:
    QString savePath;
    QFile mfile;
    QList<FileItemInfo*> files;
};

#endif // FILERECIEVERMODEL_H
