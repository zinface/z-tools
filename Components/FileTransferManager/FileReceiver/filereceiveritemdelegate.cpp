#include "filereceiveritemdelegate.h"
#include "../fileiteminfo.h"

#include <QDragEnterEvent>
#include <QPainterPath>
#include <QPainter>
#include <QStyledItemDelegate>
#include <QTextStream>

FileReceiverItemDelegate::FileReceiverItemDelegate(QObject *parent) : QItemDelegate(parent)
{

}

void FileReceiverItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid()) return;

    const QString filename = index.data(FileItemInfo::FileReceiverNameRole).toString();
    const QString filepath = index.data(FileItemInfo::FileReceiverPathRole).toString();
    const QString fileSize = index.data(FileItemInfo::FileReceiverSizeStringRole).toString();
    const qint64 filesize = static_cast<qint64>(index.data(FileItemInfo::FileReceiverSizeRole).toLongLong());
    const FileItemInfo::FileDonwloadStat downState = (FileItemInfo::FileDonwloadStat)index.data(FileItemInfo::FileReceiverDownloadRole).toInt();
    const qint64 downloadsize = static_cast<qint64>(index.data(FileItemInfo::FileReceiverDownloadSize).toLongLong());
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
    QRect sizeRect = painter->fontMetrics().boundingRect(fileSize);; // 文件大小矩形
    QRectF size_rect = QRect(rect.right() - sizeRect.width() - 35, rect.top() + (20 - sizeRect.height()/2), sizeRect.width(), sizeRect.height());
    painter->drawText(size_rect, Qt::AlignVCenter, fileSize);

    if (downState == FileItemInfo::NOT_DOWNLOAD) {
        QPixmap p(QIcon("://images/cd.svg").pixmap(QSize(20,20)));
        QRectF tagRect = QRectF(rect.right() - 30, rect.top() + 10, 20, 20);
        painter->drawImage(tagRect,p.toImage());
    } else if (downState == FileItemInfo::SIZEWARRING){
        QPixmap p(QIcon("://images/warring.svg").pixmap(QSize(20,20)));
        QRectF tagRect = QRectF(rect.right() - 30, rect.top() + 10, 20, 20);
        painter->drawImage(tagRect,p.toImage());
    } else if (downState == FileItemInfo::DOWNLOADING) {
        // 绘制饼的背景
        QRectF progressBar = QRectF(rect.right() - 30, rect.top() + 10, 20, 20);
        painter->setBrush(QColor(Qt::white));
        painter->setPen(QPen(Qt::black, 1));
        painter->drawEllipse(progressBar);

        double max(filesize), min(0);
        double value = downloadsize;
        double arcStep = 360.0 / (max - min) * value;
        
        // 绘制值
        QPainterPath basePath;
        basePath.moveTo(progressBar.center());
        basePath.arcTo(progressBar, 90, -arcStep);
        basePath.lineTo(progressBar.center());
        painter->setBrush(QColor("#0096ff"));
        painter->setPen(QPen(Qt::black, 1));
        painter->drawPath(basePath);

        QRectF topRect = QRectF(progressBar.left() + 5, progressBar.top() + 5, 10, 10);
        painter->setBrush(QColor(Qt::white));
        painter->setPen(QPen(Qt::black, 1));
        painter->drawEllipse(topRect);

    } else if (downState == FileItemInfo::DOWNLOADED) {
        QPixmap p(QIcon("://images/ok.svg").pixmap(QSize(20,20)));
        QRectF tagRect = QRectF(rect.right() - 30, rect.top() + 10, 20, 20);
        painter->drawImage(tagRect,p.toImage());
    }
    painter->restore();
}

QSize FileReceiverItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QSize(option.rect.width(), 40);
}

bool FileReceiverItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    bool rPaint = false;
    // 构造一个矩形区域
    QRect decorationRect = QRect(option.rect.right() - 30, option.rect.top() + 10, 20, 20);

    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 将事件转换为鼠标事件

    // 判断当前事件是鼠标按钮事件，并且鼠标位置是属于当前矩形范围内，就发送downloadItem信号
    if (event->type() == QEvent::MouseButtonPress && decorationRect.contains(mouseEvent->pos()))
    {
        emit downloadItem(index);
        rPaint = true;
    }
    // 返回编辑事件
    return rPaint;
}

void FileReceiverItemDelegate::drawProgress(QPainter &p, QRectF progressBar, double arcStep)
{

}
