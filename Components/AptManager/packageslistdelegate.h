#ifndef PACKAGESLISTDELEGATE_H
#define PACKAGESLISTDELEGATE_H

#include <QAbstractItemDelegate>
#include <QItemDelegate>
#include <QLabel>
#include <QWidget>

class PackagesListDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit PackagesListDelegate(QObject *parent = nullptr);

signals:

public slots:
    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
//    QLabel *pName;
//    QLabel *pVersion;

    // QAbstractItemDelegate interface
public:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
};

#endif // PACKAGESLISTDELEGATE_H
