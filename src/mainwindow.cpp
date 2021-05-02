#include "mainwindow.h"

#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QTableView>
#include <QVBoxLayout>

#include <filetransfersender.h>
#include <filetransferreceiver.h>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
  ,m_mainLayout(new QVBoxLayout(this))
  ,m_tabWidget(new QTabWidget(this))

{
    m_tabWidget->addTab(new FileTransferSender, "文件传输发送者");
    m_tabWidget->addTab(new FileTransferReceiver, "文件传输接收者");
    m_mainLayout->addWidget(m_tabWidget);
    setLayout(m_mainLayout);
    layout()->setSizeConstraint(QLayout::SetFixedSize);
}
