#ifndef FILESENDERITEMDELEGATE_H
#define FILESENDERITEMDELEGATE_H

#include <QItemDelegate>



class FileSenderItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    FileSenderItemDelegate(QObject *parent = nullptr);

    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    // QAbstractItemDelegate interface
public:
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // FILESENDERITEMDELEGATE_H
