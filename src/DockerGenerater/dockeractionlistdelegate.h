#ifndef DOCKERACTIONLISTDELEGATE_H
#define DOCKERACTIONLISTDELEGATE_H

#include <QAbstractItemDelegate>
#include <QWidget>

class DockerActionListDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    explicit DockerActionListDelegate(QObject *parent = nullptr);

signals:

public slots:

    // QAbstractItemDelegate interface
private:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QString m_actionName;
    QString m_actionValue;
};

#endif // DOCKERACTIONLISTDELEGATE_H
