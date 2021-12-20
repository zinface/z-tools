#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStringListModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 向交换机注册动作
    registerActions();
    initSignalSlots();

    // 向交换机设置工作模式：CLIENT
    manager.setWorkMode("localhost",9999,SessionManager::CLIENT);
    tree =  ui->treeView;
    tree->setModel(&m_filemode);
    ui->listView->setModel(&m_servermode);
    connect(ui->actionAddServer, &QAction::triggered, this, &MainWindow::addServer);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::registerActions()
{
    manager.registerAction(DirectoryActions::InfoMessage);
    manager.registerAction(DirectoryActions::FetchWorkFileList, true);
}

void MainWindow::initSignalSlots()
{
    connect(&manager, &InfomationManager::onReply, this, &MainWindow::onRemoteReply);
    connect(&manager, &InfomationManager::onReplyRaw, this, &MainWindow::onRemoteReplyRaw);
    connect(&manager, &InfomationManager::connected, this, &MainWindow::onConnected);
}

void MainWindow::onRemoteReply(qint8 action, QString msg)
{
    if (action == InfoMessage) {
        QTextStream out(stdout);
        out << "Reply: " << msg << "\n";
    }
}

void MainWindow::onRemoteReplyRaw(qint8 action, qint64 length, QByteArray &data)
{
    if (action == FetchWorkFileList) {
        m_filemode.setStringList(m_filemode.stringList() << QString::fromLocal8Bit(data));
        emit m_filemode.layoutChanged();
    }
}

void MainWindow::onConnected()
{
    manager.broadCaseAction(FetchWorkFileList);
}

void MainWindow::addServer()
{
    m_servermode.setStringList(m_servermode.stringList()<<QString("localhost:%1").arg(m_servermode.stringList().size()));
    emit m_servermode.layoutChanged();
}
