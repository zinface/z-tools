
#include "imageview.h"
#include "ui_imageview.h"
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
#include <QTextStream>
#include <QVBoxLayout>
#include <moveeater.h>
#include <scalewheeleater.h>

ImageView::ImageView(QWidget *parent) : QWidget(parent)
    , ui(new Ui::ImageView)
{
    ui->setupUi(this);

    m_dirpath_lineedit = ui->e_dirpath;
    m_image_list = ui->listWidget;
    m_image_label = ui->label;

    new MoveEater(m_image_label);
    (new ScaleWheelEater(m_image_label, m_image_label, [this](QWheelEvent * event, QSize before, QSize after)
    {
        if (m_currentPic.isNull() == false)
        {
            m_image_label->setPixmap(m_currentPic.scaled(QSize(m_image_label->width(), m_image_label->height()), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }))->setRatio(40);
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


QFileInfoList allinfos;
void ImageView::on_e_dirpath_textChanged(const QString &arg1)
{
    m_image_list->clear();
    allinfos.clear();

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
            }
        }
    }
    if (m_image_list->item(0) != nullptr)
    {
        m_image_list->setCurrentItem(0);
    }
}


void ImageView::on_listWidget_itemEntered(QListWidgetItem *item)
{

}

void ImageView::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    m_currentPic = QPixmap();
    if (!current) return;

    QPixmap temp, pic(current->text());
    QIcon ico = current->icon();

    if (pic.width() < m_image_label->width() && pic.height() < m_image_label->height())
    {
        temp = pic;
    }
    else
    {
        temp = pic.scaled(QSize(m_image_label->width(), m_image_label->height()), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    m_image_label->setPixmap(temp);
    m_currentPic = pic;

    if (ico.isNull())
    {
        current->setIcon(pic);
    }
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

