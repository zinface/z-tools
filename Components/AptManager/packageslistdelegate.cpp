#include "packageslistdelegate.h"
#include "packageviewmodel.h"

#include <QApplication>
#include <QCheckBox>
#include <QClipboard>
#include <QDragEnterEvent>
#include <QPainter>
#include <QPainterPath>
#include <QProcess>
#include <QPushButton>
#include <QStyle>
PackagesListDelegate::PackagesListDelegate(QObject *parent) : QItemDelegate (parent)
{

}

void PackagesListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

    QPen old = painter->pen();

    // ---------------------
    QRectF rect;
    rect.setX(option.rect.x());
    rect.setY(option.rect.y());
    rect.setWidth(option.rect.width());
    rect.setHeight(option.rect.height());

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
        painter->setPen(old);
    }
    // ---------------------

#if 0
    // draw package name
    QRect name_rect = option.rect;
    name_rect.setLeft(10);
    name_rect.setHeight(name_rect.height()/2);

    const QString name = index.model()->data(index).value<QStringList>().at(0);
    QFont old_font = painter->font();
    const QString name_str = painter->fontMetrics().elidedText(name, Qt::ElideRight, 200);
    const QRectF name_buinding_rect = painter->boundingRect(name_rect, name_str, Qt::AlignLeft | Qt::AlignBottom);
//    painter->setPen(Qt::black);
    painter->drawText(name_rect, name_str, Qt::AlignLeft | Qt::AlignBottom);

    // draw package version
    const int version_x = name_buinding_rect.right() + 10;
    QRect version_rect = name_rect;
    version_rect.setLeft(version_x);
    version_rect.setRight(option.rect.right() - 60);
    const QString version = index.model()->data(index).value<QStringList>().at(1);
    const QString verstion_str = painter->fontMetrics().elidedText(version, Qt::ElideRight, version_rect.width());
//    painter->setPen(Qt::black);
    painter->drawText(version_rect, verstion_str, Qt::AlignLeft | Qt::AlignBottom);
#else
     painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    // draw package name
    QRect name_rect = option.rect;
    name_rect.setLeft(10);

    QString name = index.model()->data(index, PackageViewModel::PackageNameRole).toString();
    QString name_str = painter->fontMetrics().elidedText(name, Qt::ElideRight, 210);
    painter->drawText(name_rect,name_str);

    // draw package architecture
    QRect architecture_rect = name_rect;
    architecture_rect.setLeft(230);

    QString architecture = index.model()->data(index,PackageViewModel::PackageArchitecture).toString();
    painter->drawText(architecture_rect,architecture);

    // draw package version
    QRect version_rect = architecture_rect;
    version_rect.setLeft(300);

    QString version = index.model()->data(index, PackageViewModel::PackageVersionRole).toString();
    QString version_str = painter->fontMetrics().elidedText(version, Qt::ElideRight, 140);
    painter->drawText(version_rect,version_str);

    // draw checkbox
    QRect checkbox_rect = version_rect;
    checkbox_rect.setLeft(450);

    int install_stat = index.model()->data(index, PackageViewModel::PackageInstalledVersionRole).toInt();

    QStyleOptionButton checkBoxOption;
    checkBoxOption.rect = checkbox_rect;
    checkBoxOption.state |= QStyle::State_Enabled;

    switch (install_stat) {
        case PackageViewModel::NotInstalled:
            checkBoxOption.text = "未安装";
            checkBoxOption.state |= QStyle::State_Off;
            break;
        case PackageViewModel::Installed:
            checkBoxOption.text = "已安装";
            checkBoxOption.state |= QStyle::State_On;
            break;
        case PackageViewModel::Upgradeable:
            checkBoxOption.text = "可更新";
            checkBoxOption.state |= QStyle::State_NoChange;
    }

    QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkBoxOption, painter);
    painter->restore();
#endif
}

QSize PackagesListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    return QItemDelegate::sizeHint(option, index);
}

bool PackagesListDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    bool rPaint = false;
    // 构造一个矩形区域
    QRect decorationRect = option.rect;

    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 将事件转换为鼠标事件

    // 判断当前事件是鼠标按钮事件，并且鼠标位置是属于当前矩形范围内，就发送downloadItem信号
    if (event->type() == QEvent::MouseButtonDblClick && decorationRect.contains(mouseEvent->pos()))
    {
        QString name = index.model()->data(index, PackageViewModel::PackageNameRole).toString();
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(QString("pkexec apt install %1 --yes").arg(name));
//        QProcess install;
//        install.start(QString("/usr/bin/pkexec apt install %1 --yes").arg(name));
//        install.waitForFinished();
        rPaint = true;
    }
    // 返回编辑事件
    return rPaint;
}
