#ifndef FILETRANSFERSENDER_H
#define FILETRANSFERSENDER_H

#include <QFile>
#include <QProgressBar>
#include <QQueue>
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
    /***** tcpClient *****/
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();
    void onClientChanged();

    /***** fileCtlBtns ******/
    void addFile();
    void delFile();
    void clrFile();

    /***** fileListWidget *****/
    void onfilesAppended(QStringList& files);
    void onfilesDeleted(QString file);
    void onfilesCleanded();
    void onFileListChanged();

    void onTimerout();

    /***** fileQueue *****/
    void onFilesQueueChange();

signals:
    void clientChanged();

    void filesAppended(QStringList& files);
    void filesDeleted(QString file);
    void filesCleanded();

    void filesChanged();
    void emitFilesQueueChange();

private:
    int conn_cnt = 0;
    int value = 0;
    QMap<QString, QTcpSocket*> m_tcpMap;

    QQueue<QString> m_fileQueue;

private:
    QFile m_file;
    QListWidget *fileListWidget;

    QPushButton *addFileBtn;
    QPushButton *delFileBtn;
    QPushButton *clrFileBtn;

    QTcpServer tcpServer;

    QLabel *currentLab;
    QLabel *totalLab;
    QProgressBar currentProgressBar;
    QProgressBar totalProgressBar;

    QLabel *listenPort;
    QLabel *statusBar;
    QLabel *clientStatus;

    QLabel *filesQueueStatus;

private:
    void createFileTransferSender();

};

#endif // FILETRANSFERSENDER_H
