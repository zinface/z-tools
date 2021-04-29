#ifndef FILERECEIVERVIEW_H
#define FILERECEIVERVIEW_H

#include <QListView>

class FileReceiverItemDelegate;
class FileReceiverModel;

class FileReceiverView : public QListView
{
    Q_OBJECT
public:
    explicit FileReceiverView(QWidget *parent = nullptr);

public slots:

    void appendFile(const QString &fileName, qint64 filesize);
    void appendFile(const QString &fileName);
    void appendFilse(const QStringList &files);
    void onclicked(const QModelIndex &index);
    FileReceiverModel *getMode();

public:
    int currentRow = -1;

private:
    FileReceiverItemDelegate *mfileReceiverItemDelegate;
    FileReceiverModel *mfileReciverModel;

};

#endif // FILERECEIVERVIEW_H
