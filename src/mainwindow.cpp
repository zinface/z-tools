#include "mainwindow.h"

#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QTableView>
#include <QVBoxLayout>
#include <templatewidget.h>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
  ,m_mainLayout(new QVBoxLayout(this))
  ,m_tabWidget(new QTabWidget(this))

{

    m_tabWidget->addTab(new TemplateWidget(""), "0");
    m_tabWidget->addTab(new TemplateWidget("1"), "1");
    m_tabWidget->addTab(new TemplateWidget("2"), "2");
    m_tabWidget->addTab(new TemplateWidget("3"), "3");

    m_mainLayout->addWidget(m_tabWidget);
    setLayout(m_mainLayout);
//    layout()->setSizeConstraint(QLayout::SetFixedSize);
}
