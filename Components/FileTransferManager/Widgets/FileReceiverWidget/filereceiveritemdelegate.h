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
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    void drawProgress(QPainter &p, QRectF progressBar, double arcStep);
    void drawDownload();

signals:
    void downloadItem(const QModelIndex &);
};

#endif // FILERECEIVERITEMDELEGATE_H
