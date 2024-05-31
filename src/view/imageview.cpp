
#include "imageview.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QTextStream>
#include <QVBoxLayout>

ImageView::ImageView(QWidget *parent) : QWidget(parent)
  ,m_dirpath_lineedit(new QLineEdit)
  ,m_image_list(new QListWidget)
  ,m_image_label(new QLabel)
{
    initUi();

    connect(m_dirpath_lineedit, &QLineEdit::textChanged, this, &ImageView::onChangePath);
    connect(m_image_list, &QListWidget::itemEntered, this, &ImageView::onSelectImageItem);
    connect(m_image_list, &QListWidget::itemClicked, this, &ImageView::onSelectImageItem);
}
ImageView::~ImageView()
{

}

void ImageView::initUi()
{
    QVBoxLayout *imageinfo_layout = new QVBoxLayout;
    imageinfo_layout->addWidget(m_dirpath_lineedit);
    imageinfo_layout->addWidget(m_image_list);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(imageinfo_layout);
    mainLayout->addWidget(m_image_label);

    mainLayout->setStretch(0, 3);
    mainLayout->setStretch(1, 7);

    setLayout(mainLayout);
    resize(650,400);

    m_image_label->setMinimumSize(50,30); // 为防止最小大小自动变大，设置最小大小
    m_image_label->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    m_image_label->setStyleSheet(" QLabel{ border: 2px solid green; border-radius: 4px; padding: 1px; }");
}

QFileInfoList ImageView::loadFileInfos(QString path)
{
    // QTextStream(stdout) << QString("%1\n").arg(path);
    m_currentpaht = path;
    QDir currentDir(path);
    QStringList fileList;
    QFileInfoList infoList = currentDir.entryInfoList(fileList, QDir::Dirs | QDir::NoDotDot | QDir::NoDot, QDir::DirsFirst);

    QFileInfoList tempList = currentDir.entryInfoList(fileList, QDir::Files);

    for (int i = 0;i<infoList.size(); i++) {
        QFileInfo info  = infoList.at(i);
        if (info.isDir()) {
            tempList.append(loadFileInfos(info.absoluteFilePath()));
        }
    }
    return tempList;
}

QFileInfoList allinfos;
void ImageView::onChangePath()
{
    m_image_list->clear();
    allinfos.clear();

    QString path = m_dirpath_lineedit->text();
    QFileInfo filepath(path);
    if (filepath.exists() && filepath.isReadable() && filepath.isDir()) {
        allinfos = loadFileInfos(path);

        for (int i = 0;i<allinfos.size(); i++) {
            QFileInfo info  = allinfos.at(i);
            QString filename = info.fileName();
//            QListWidgetItem *item = new QListWidgetItem(filename);
            QListWidgetItem *item = new QListWidgetItem(info.absoluteFilePath());
            QString suffix = info.suffix();
            auto avaliable = [=]{
                return
                suffix.indexOf("jpg") == 0 ||
                suffix.indexOf("png") == 0 ||
                suffix.indexOf("svg") == 0 ;
            };
            if (avaliable()) {
                m_image_list->addItem(item);
            }
        }
    }
    if (m_image_list->item(0) != nullptr) {
        emit onSelectImageItem(m_image_list->item(0));
    }
}

void ImageView::onSelectImageItem(QListWidgetItem *item)
{
    QPixmap temp, pic(item->text());
    QIcon ico = item->icon();

    if (pic.width() < m_image_label->width() && pic.height() < m_image_label->height()) {
        temp = pic;
    } else {
        temp = pic.scaled(QSize(m_image_label->width(), m_image_label->height()), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    m_image_label->setPixmap(temp);

    if (ico.isNull()) {
        item->setIcon(pic);
    }
}

void ImageView::onResize()
{

}




























