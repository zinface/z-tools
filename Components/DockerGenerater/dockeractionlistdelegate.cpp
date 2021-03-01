#include "dockeractionlistdelegate.h"

DockerActionListDelegate::DockerActionListDelegate(QObject *parent)
    : QAbstractItemDelegate(parent)
{

}

void DockerActionListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // 设置起点坐标
//    QPoint start();
    // 设置结束坐标
//    QPoint end();

}

QSize DockerActionListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{

}
