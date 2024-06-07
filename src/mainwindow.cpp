#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <screenutil.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ScreenUtil::moveCenterForCursor(*this);

    connect(ui->widget, &ImageView::messageEvent, this, [this](const QString & message)
    {
        ui->statusbar->showMessage(message);
    });

    ui->widget->setWindowHandler(this->windowHandle());

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loaderDirectory(const QString &dirpath)
{
    ui->widget->setDefaultPath(dirpath);
}
