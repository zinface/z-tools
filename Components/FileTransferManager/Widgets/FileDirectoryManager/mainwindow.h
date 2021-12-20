#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QStringListModel>
#include <../../FileDirectory/InfomationManager/infomationmanager.h>
#include <../../FileDirectory/DirectoryActions.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void registerActions();
    void initSignalSlots();

private slots:
    void onRemoteReply(qint8 action, QString msg);
    void onRemoteReplyRaw(qint8 action, qint64 length, QByteArray &data);
    void onConnected();

    void addServer();

private:
    Ui::MainWindow *ui;
    QTreeView *tree;
    QStringListModel m_filemode;
    QStringListModel m_servermode;

    InfomationManager manager;
};

#endif // MAINWINDOW_H
