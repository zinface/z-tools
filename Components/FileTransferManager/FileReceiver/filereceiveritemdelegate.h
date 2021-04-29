#ifndef FILERECEIVERITEMDELEGATE_H
#define FILERECEIVERITEMDELEGATE_H

#include <QItemDelegate>

class FileReceiverItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit FileReceiverItemDelegate(QObject *parent = nullptr);

    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // FILERECEIVERITEMDELEGATE_H
