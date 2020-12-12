#include "mainwindow.h"

#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QTableView>
#include <QVBoxLayout>

#include <adbmanager.h>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
  ,m_mainLayout(new QVBoxLayout(this))
  ,m_tabWidget(new QTabWidget(this))

{

    m_tabWidget->addTab(new AdbManager, "设备信息管理");

    m_mainLayout->addWidget(m_tabWidget);
    setLayout(m_mainLayout);
    layout()->setSizeConstraint(QLayout::SetFixedSize);
}
