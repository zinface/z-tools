#ifndef FILETRANSFERSENDER_H
#define FILETRANSFERSENDER_H

#include <QFile>
#include <QProgressBar>
#include <QTcpServer>
#include <QWidget>

class QLabel;
class QListWidget;
class QPushButton;

class FileTransferSender : public QWidget
{
    Q_OBJECT
public:
    explicit FileTransferSender(QWidget *parent = nullptr);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();
    void onClientChanged();

    void addFile();
    void delFile();
    void clrFile();

    void onFileListChanged();
    void onTimerout();

signals:
    void clientChanged();

    void filesAppended();
    void filesDeleted();
    void filesCleanded();

    void filesChanged();

private:
    int conn_cnt = 0;
    int value = 0;
    QMap<QString, QTcpSocket*> m_tcpMap;

private:
    QFile m_file;
    QListWidget *listWidget;

    QPushButton *addFileBtn;
    QPushButton *delFileBtn;
    QPushButton *clrFileBtn;

    QTcpServer tcpServer;

    QLabel *listenPort;
    QLabel *statusBar;
    QLabel *clientStatus;

    QLabel *currentLab;
    QLabel *totalLab;
    QProgressBar currentProgressBar;
    QProgressBar totalProgressBar;

private:
    void createFileTransferSender();

};

#endif // FILETRANSFERSENDER_H
