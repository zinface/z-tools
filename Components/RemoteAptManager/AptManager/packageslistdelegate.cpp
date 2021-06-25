#include "packageslistdelegate.h"
#include "packageviewmodel.h"

#include <QApplication>
#include <QCheckBox>
#include <QPainter>
#include <QPushButton>
#include <QStyle>
PackagesListDelegate::PackagesListDelegate(QObject *parent) : QItemDelegate (parent)
{

}

void PackagesListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

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

    if(install_stat == PackageViewModel::NotInstalled)
    {
        checkBoxOption.text = "未安装";
        checkBoxOption.state |= QStyle::State_Off;
    } else {
        checkBoxOption.text = "已安装";
        checkBoxOption.state |= QStyle::State_On;
    }

    QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkBoxOption, painter);
    painter->restore();
#endif
}

QSize PackagesListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    return QItemDelegate::sizeHint(option, index);
}
