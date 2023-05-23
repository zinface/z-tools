#include "mainwindow.h"

#include <QFileInfo>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QTableView>
#include <QVBoxLayout>
#include <apkmanager.h>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
  ,m_mainLayout(new QVBoxLayout(this))
  ,m_tabWidget(new QTabWidget(this))
  ,apkManager(new ApkManager)

{

    m_tabWidget->addTab(apkManager, "apk文件查看器");

    m_mainLayout->addWidget(m_tabWidget);
    setLayout(m_mainLayout);
//    layout()->setSizeConstraint(QLayout::SetFixedSize);
    layout()->setSizeConstraint(QLayout::SetDefaultConstraint);
}

/**
 * @brief 选中一个 apk 文件
 * @param apkFile
 */
void MainWindow::switchApkFile(QString apkFile)
{
    QFileInfo apkInfo(apkFile);
    // 如果该文件存在并且是一个 apk 文件
    if (apkInfo.exists() && apkInfo.isFile() && apkFile.endsWith(".apk")) {
        apkManager->chooseApk(apkFile);
    }
}
