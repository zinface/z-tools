#include "filereceiveritemdelegate.h"
#include "../fileiteminfo.h"

#include <QPainter>

FileReceiverItemDelegate::FileReceiverItemDelegate(QObject *parent) : QItemDelegate(parent)
{

}

void FileReceiverItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid()) return;

    const QString filename = index.data(FileItemInfo::FileSenderNameRole).toString();
    const QString filepath = index.data(FileItemInfo::FileSenderPathRole).toString();
    const QString filesize = index.data(FileItemInfo::FileSenderSizeRole).toString();
//    const QString fileupState = index.data(FileItemInfo::FileSenderUploadRole).toString();
    painter->save();

    // item 矩形区域
    QRectF rect;
    rect.setX(option.rect.x());
    rect.setY(option.rect.y());
    rect.setWidth(option.rect.width()-1);
    rect.setHeight(option.rect.height()-1);

    QPainterPath path;
    path.moveTo(rect.topRight());
    path.lineTo(rect.topLeft());
    path.quadTo(rect.topLeft(), rect.topLeft());
    path.lineTo(rect.bottomLeft());
    path.quadTo(rect.bottomLeft(), rect.bottomLeft());
    path.lineTo(rect.bottomRight());
    path.quadTo(rect.bottomRight(), rect.bottomRight());
    path.lineTo(rect.topRight());
    path.quadTo(rect.topRight(), rect.topRight());

    // 鼠标悬停或者选中时改变背景色
    if (option.state.testFlag(QStyle::State_MouseOver)) {
        painter->setPen(QPen(QColor("#e9e9ec")));
        painter->setBrush(QColor("#e9e9ec"));
        painter->drawPath(path);
    }

    // 条目被选中颜色
    if(option.state.testFlag(QStyle::State_Selected)) {
        painter->setPen(QPen(QColor("#6791aa")));
        painter->setBrush(QColor("#6791aa"));
        painter->drawPath(path);
        painter->setPen(Qt::white);
    } else {
        painter->setPen(Qt::black);
    }

    // 绘制文件名或文件路径
    QRect nameOrpathRect;
    QString name_str = filename;
    // 如果是选中部分
    if(option.state.testFlag(QStyle::State_Selected)) {
        // 将显示路径
        name_str = filename;
        // 获取filepath的矩形
        nameOrpathRect = painter->fontMetrics().boundingRect(filename);
        // 如果文本宽度大于2倍容纳宽度，将变更为只显示文件名
        if (nameOrpathRect.width() > rect.width() * 1.6) {
            name_str = filename;
        }
    }
    QRectF nameOrpath_rect = QRect(rect.left() + 5, rect.top(), rect.width() * 0.8, rect.height());
    painter->drawText(nameOrpath_rect, Qt::TextWordWrap | Qt::AlignVCenter, name_str);

    // 绘制文件大小
    QRect sizeRect = painter->fontMetrics().boundingRect(filesize);; // 文件大小矩形
    QRectF size_rect = QRect(rect.right() - sizeRect.width() - 5, rect.top() + (20 - sizeRect.height()/2), sizeRect.width(), sizeRect.height());
    painter->drawText(size_rect, Qt::AlignVCenter, filesize);
    painter->restore();
}

QSize FileReceiverItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QSize(option.rect.width(), 40);
}
