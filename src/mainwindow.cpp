#include "mainwindow.h"

#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QTableView>
#include <QVBoxLayout>

#include <desktopgenerater.h>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
  ,m_mainLayout(new QVBoxLayout(this))
  ,m_tabWidget(new QTabWidget(this))
  ,m_desktopGenerater(new DesktopGenerater(this))

{

    m_tabWidget->addTab(m_desktopGenerater, "桌面图标生成器");

    m_mainLayout->addWidget(m_tabWidget);
    setLayout(m_mainLayout);
    layout()->setSizeConstraint(QLayout::SetFixedSize);
}
