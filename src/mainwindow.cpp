#include "mainwindow.h"

#include <QFileInfo>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QTableView>
#include <QVBoxLayout>
#include <apkmanager.h>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
    , m_tabWidget(new QTabWidget(this))
    , apkManager(new ApkManager)
    , m_mainLayout(new QVBoxLayout(this))
{
    setLayout(new QVBoxLayout(this));

    initUI();
}

void MainWindow::initUI()
{
    initTab();
    m_mainLayout->addWidget(m_tabWidget);
    layout()->setSizeConstraint(QLayout::SetDefaultConstraint);
}

void MainWindow::initTab()
{
    m_tabWidget->addTab(apkManager, "apk文件查看器");
}

/**
 * @brief 选中一个 apk 文件
 * @param apkFile
 */
void MainWindow::switchApkFile(const QString &apkFile)
{
    QFileInfo apkInfo(apkFile);
    // 如果该文件存在并且是一个 apk 文件
    if (apkInfo.exists()
        && apkInfo.isFile()
        && (apkFile.endsWith(".apk") || (apkFile.contains(".apk") && apkFile.endsWith(".1") ))) {
        apkManager->chooseApk(apkFile);
    }
}
