
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
#include <QKeyEvent>
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

#include <manager/loadermanager.h>

#define ITEM_DATA_PATH 990

ImageView::ImageView(QWidget *parent) : QWidget(parent)
    , ui(new Ui::ImageView)
    , manager(new LoaderManager)
{
    ui->setupUi(this);

    manager = new LoaderManager;
    connect(manager, &LoaderManager::Thumbnail, this, [this](const QString &current, const QPixmap &pixmap){
        for (int var = 0; var < ui->listWidget->count(); ++var) {
            if (auto item = ui->listWidget->item(var)) {
                if (item->text() == current) {
                    item->setIcon(pixmap);
                    return;
                }
            }
        }
    });
    connect(manager, &LoaderManager::IndexThumbnail, this, [this](int i, const QString &current, const QPixmap &pixmap){
        if (auto item = ui->listWidget->item(i)) {
            if (item->text() == current) {
                item->setIcon(pixmap);
                return;
            }
        }
    });
    manager->start();

    m_dirpath_lineedit = ui->e_dirpath;
    m_image_list = ui->listWidget;
    m_image_label = new QLabel;

    connect(ui->widget, &SuperImage::messageEvent, this, &ImageView::messageEvent);

//    static int bx = 0, by = 0;
//    static double pw = 0, ph = 0;

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
    if (manager) manager->stop();
    if (manager) manager->deleteLater();
    manager = nullptr;
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
    manager->setWorkStop(true);
    m_image_list->clear();
    allinfos.clear();
    avaliables = 0;

    QString path = m_dirpath_lineedit->text();
    QFileInfo filepath(path);
    if (filepath.exists() && filepath.isReadable() && filepath.isDir())
    {
        allinfos = loadFileInfos(path);

        manager->setWorkStop(false);

        for (int i = 0; i < allinfos.size(); i++)
        {
            QFileInfo info  = allinfos.at(i);
            QString filename = info.fileName();
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
                QListWidgetItem *item = new QListWidgetItem();
                QString filepath = info.absoluteFilePath();
                item->setData(ITEM_DATA_PATH, info.absoluteFilePath());
//                item->setIcon(QPixmap(filepath).scaled(QSize(50, 50)));

                QString viewText = filepath;

                viewText.replace(arg1, "");
                if (viewText[0] == '/' && viewText.length() > 2)
                {
                    viewText.prepend(".");
                }
                else
                {
                    viewText.prepend("./");
                }
                item->setText(viewText);
//                emit manager->requestThumbnail(viewText, filepath);
                emit manager->requestIndexThumbnail(avaliables, viewText, filepath);

                m_image_list->addItem(item);
                avaliables++;
                emit messageEvent(QString("发现 %1 张图片").arg(avaliables));
            }
        }
    }

    filtering_items(ui->e_find->text());

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
    if (!current)
        return;

    QPixmap pixmap(current->data(ITEM_DATA_PATH).toString());
    // 存储一份图片数据用于复制操作
    m_currentPic = pixmap;

    if (current->icon().isNull() == false)
    {
        // 缩放一下可降低内存使用量
        current->setIcon(pixmap.scaled(QSize(50, 50)));
    }

    emit messageEvent("正在预览: " + current->data(ITEM_DATA_PATH).toString());

    refresh();
}

void ImageView::on_widget_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction("复制", [this]()
    {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setPixmap(m_currentPic.copy());
    });

    menu.addAction("打开文件位置", [this]()
    {
        DBusUtil::showFileLocation(ui->listWidget->currentItem()->data(ITEM_DATA_PATH).toString());
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

    ui->widget->setPixmap(m_currentPic);
}

void ImageView::setWindowHandler(QWindow *newWindowHandler)
{
    m_windowHandler = newWindowHandler;
}

void ImageView::on_e_find_textChanged(const QString &arg1)
{
    filtering_items(arg1);
}

void ImageView::filtering_items(const QString &content)
{
    int i = 0;
    while (QListWidgetItem *item = m_image_list->item(i++))
    {
        if (content.isEmpty())
        {
            item->setHidden(false);
            continue;
        }

        if (item->text().contains(content, Qt::CaseInsensitive))
        {
            item->setHidden(false);
        }
        else
        {
            item->setHidden(true);
        }
    }
}

void ImageView::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_C)
    {
        if (ui->listWidget->hasFocus())
        {
            if (auto item = ui->listWidget->currentItem())
            {
                QString filepath = item->data(ITEM_DATA_PATH).toString();
                QClipboard *clipboard = QGuiApplication::clipboard();
                clipboard->setText(filepath);
                emit messageEvent(QString("已复制: %1").arg(filepath));
            }
        }

        if (ui->widget->hasFocus())
        {
            if (auto item = ui->listWidget->currentItem())
            {
                QString filepath = item->data(ITEM_DATA_PATH).toString();
                QClipboard *clipboard = QGuiApplication::clipboard();
                clipboard->setPixmap(filepath);
                emit messageEvent(QString("已复制图片数据"));
            }
        }
    }

    if (event->key() == Qt::Key_Up)
    {
        ui->listWidget->setCurrentRow(ui->listWidget->currentRow() - 1);
    }

    if (event->key() == Qt::Key_Down)
    {
        ui->listWidget->setCurrentRow(ui->listWidget->currentRow() + 1);
    }
}

