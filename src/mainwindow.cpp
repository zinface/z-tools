#include "mainwindow.h"

#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QTableView>
#include <QVBoxLayout>

#include <aptmanager.h>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
  ,m_mainLayout(new QVBoxLayout(this))
  ,m_tabWidget(new QTabWidget(this))

{

    m_tabWidget->addTab(new AptManager, "软件包查看器");

    m_mainLayout->addWidget(m_tabWidget);
    setLayout(m_mainLayout);
    layout()->setSizeConstraint(QLayout::SetFixedSize);
}
