
#include "imageview.h"
#include "ui_imageview.h"
#include "utils/ScreenUtils.h"
#include "utils/dbusutil.h"

#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMenu>
#include <QScreen>
#include <QTextStream>
#include <QVBoxLayout>
#include <QWheelEvent>
#include <QWindow>
#include <moveeater.h>
#include <scalewheeleater.h>

int bx = 0, by = 0;
double pw = 0, ph = 0;
ImageView::ImageView(QWidget *parent) : QWidget(parent)
    , ui(new Ui::ImageView)
{
    ui->setupUi(this);

    m_dirpath_lineedit = ui->e_dirpath;
    m_image_list = ui->listWidget;
    m_image_label = new QLabel;

//    new MoveEater(m_image_label);
//    (new ScaleWheelEater(m_image_label, m_image_label, [this](QWheelEvent * event, QSize before, QSize after)
//    {
//        const QPixmap *pixmap = m_image_label->pixmap();
//        if (pixmap && pixmap->isNull() == false) {
//            qreal ratio = ScreenUtils::ratio(this);
//            pw = pixmap->width() * ratio;
//            ph = pixmap->height() * ratio;
//        }
//        bx = m_image_label->x();
//        by = m_image_label->y();
//    },
//    [this](QWheelEvent * event, QSize before, QSize after)
//    {
//        if (m_currentPic.isNull() == false)
//        {
//            QPixmap pixmap = m_currentPic;
//            qreal ratio = ScreenUtils::ratio(this);
//            pixmap.setDevicePixelRatio(ratio);
//            QSize render = after * ratio;

//            pixmap = pixmap.scaled(render, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//            m_image_label->setPixmap(pixmap);
//        }

//        QPointF oldpos = event->position();
//        QPointF newpos;

//        double sw = double(after.width()) / double(before.width());
//        qreal sh = double(after.height()) / double(before.height());

//        newpos.setX(oldpos.x() * sw);
//        newpos.setY(oldpos.y() * sh);

//        auto pos = newpos - oldpos;
//        m_image_label->move(-pos.x() + bx, -pos.y() + by);

//    }))->setRatio(40);
}
ImageView::~ImageView()
{
    delete ui;
}

QFileInfoList ImageView::loadFileInfos(QString path, int depth)
{
    int m_depth = depth++;
    if (m_depth > ui->sp_maxdepth->value())
    {
        return QFileInfoList();
    }
    // QTextStream(stdout) << QString("%1\n").arg(path);
    m_currentpaht = path;
    QDir currentDir(path);
    QStringList fileList;
    QFileInfoList infoList = currentDir.entryInfoList(fileList, QDir::Dirs | QDir::NoDotDot | QDir::NoDot, QDir::DirsFirst);

    QFileInfoList tempList = currentDir.entryInfoList(fileList, QDir::Files);

    for (int i = 0; i < infoList.size(); i++)
    {
        QFileInfo info  = infoList.at(i);
        if (info.isDir())
        {
            tempList.append(loadFileInfos(info.absoluteFilePath(), depth));
        }
    }
    return tempList;
}

void ImageView::setDefaultPath(const QString &path)
{
    ui->e_dirpath->setText(path);
}

QFileInfoList allinfos;
int avaliables;
void ImageView::on_e_dirpath_textChanged(const QString &arg1)
{
    m_image_list->clear();
    allinfos.clear();
    avaliables = 0;

    QString path = m_dirpath_lineedit->text();
    QFileInfo filepath(path);
    if (filepath.exists() && filepath.isReadable() && filepath.isDir())
    {
        allinfos = loadFileInfos(path);

        for (int i = 0; i < allinfos.size(); i++)
        {
            QFileInfo info  = allinfos.at(i);
            QString filename = info.fileName();
            //            QListWidgetItem *item = new QListWidgetItem(filename);
            QListWidgetItem *item = new QListWidgetItem(info.absoluteFilePath());
            QString suffix = info.suffix();
            auto avaliable = [ = ]
            {
                return
                suffix.indexOf("jpg") == 0 ||
                suffix.indexOf("png") == 0 ||
                suffix.indexOf("svg") == 0 ;
            };
            if (avaliable())
            {
                m_image_list->addItem(item);
                avaliables++;
                emit messageEvent(QString("发现 %1 张图片").arg(avaliables));
            }
        }
    }

    if (m_image_list->item(0) != nullptr)
    {
        m_image_list->setCurrentRow(0);
    }
}


void ImageView::on_listWidget_itemEntered(QListWidgetItem *item)
{

}

void ImageView::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    m_currentPic = QPixmap();
    if (!current) return;

    emit messageEvent("正在预览: " + current->text());

    bool item_icon_empty = current->icon().isNull();

    QPixmap pixmap(current->text());
    // 存储一份图片数据用于复制操作
    m_currentPic = pixmap;

    if (item_icon_empty)
    {
        // 缩放一下可降低内存使用量
        current->setIcon(pixmap.scaled(QSize(50,50)));
    }

    refresh();
}

void ImageView::on_label_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction("复制", [this]()
    {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setPixmap(m_currentPic.copy());
    });

    menu.addAction("打开文件位置", [this]()
    {
        DBusUtil::showFileLocation(ui->listWidget->currentItem()->text());
    });

    if (m_currentPic.isNull() == false)
    {
        menu.exec(QCursor::pos());
    }
}

void ImageView::on_sp_maxdepth_valueChanged(int arg1)
{
    on_e_dirpath_textChanged(ui->e_dirpath->text());
}

void ImageView::refresh()
{
    if (m_currentPic.isNull())
        return;

//    QPixmap pixmap = m_currentPic;
//    qreal ratio = ScreenUtils::ratio(this);
//    pixmap.setDevicePixelRatio(ratio);

//    QSize render = m_image_label->size() * ratio - QSize(9,9);

//    if (pixmap.width() < render.width() && pixmap.height() < render.height())
//    {
//        pixmap = pixmap;
//    }
//    else
//    {
//        pixmap = pixmap.scaled(render, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//    }

//    m_image_label->setPixmap(pixmap);
    ui->widget->setPixmap(m_currentPic);
}

void ImageView::setWindowHandler(QWindow *newWindowHandler)
{
    m_windowHandler = newWindowHandler;
}

