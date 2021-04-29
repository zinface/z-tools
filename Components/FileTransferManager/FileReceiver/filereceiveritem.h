#ifndef FILERECEIVERITEM_H
#define FILERECEIVERITEM_H

#include <QItemDelegate>
#include <QLabel>
#include <QObject>

class FileReceiverItem : public QItemDelegate
{
    Q_OBJECT
public:
    explicit FileReceiverItem(QObject *parent = nullptr);

private:
    void initFileReceiverItem();

private:
    QLabel mfileName;
    QLabel mFileStat;

};

#endif // FILERECEIVERITEM_H
