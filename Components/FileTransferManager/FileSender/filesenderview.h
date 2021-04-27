#ifndef FILESENDERVIEW_H
#define FILESENDERVIEW_H

#include <QListView>

class FileSenderItemDelegate;
class FileSenderModel;
class FileItemInfo;

class FileSenderView : public QListView
{
    Q_OBJECT
public:
    explicit FileSenderView(QWidget *parent = nullptr);

public slots:
    void appendFilse(const QStringList &files);
    void filesDeleted(QString file);
    void filesCleanded();
    void onclicked(const QModelIndex &index);
    int count();
    FileItemInfo *item(int i);
    FileSenderModel *getModel();


public:
    int currentRow = -1;

private:
    FileSenderItemDelegate *mfileSenderItemDelegate;
    FileSenderModel *mfileSenderModel;
};

#endif // FILESENDERVIEW_H
