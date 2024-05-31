#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <utils/ScreenUtils.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ScreenUtils::moveCenterForCursor(*this);

    connect(ui->widget, &ImageView::currentFileNameChanged, this, [this](const QString & filename)
    {
        ui->statusbar->showMessage("正在预览: " + filename);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
