#include "apkiconshowmainwindow.h"
#include "ui_apkiconshowmainwindow.h"

#include <QPainter>

ApkIconShowMainWindow::ApkIconShowMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ApkIconShowMainWindow)
{
    ui->setupUi(this);

    tmpAppIcon = "/tmp/apk-view-icon.png";

    createPlaceholderIcon();

    ui->label->setPixmap(tmpAppIcon);
}

ApkIconShowMainWindow::~ApkIconShowMainWindow()
{
    delete ui;
}


void ApkIconShowMainWindow::createPlaceholderIcon()
{
    QSize size(64,64);
    QPixmap pixmap(size);
    pixmap.fill(QColor(200, 200, 200));

    QPainter painter(&pixmap);
    painter.setPen(Qt::darkGray);
    painter.drawRect(0, 0, size.width()-1, size.height()-1);
    painter.drawText(pixmap.rect(), Qt::AlignCenter, "No Icon");

    pixmap.save(tmpAppIcon);
}
