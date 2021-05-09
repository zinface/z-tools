#ifndef FILETRANSFERRECEIVER_H
#define FILETRANSFERRECEIVER_H

#include <QFile>
#include <QTcpSocket>
#include <QWidget>

#include "fileiteminfo.h"

class QComboBox;
class QLabel;
class QLineEdit;
class QListWidget;
class QProgressBar;
class QPushButton;
class ScanWorkerPool;
class FileReceiverView;
class FileSessionAdapter;
class FileTransferManager;

class FileTransferReceiver : public QWidget
{
    Q_OBJECT
public:
    explicit FileTransferReceiver(QWidget *parent = nullptr);


private:
//    QTcpSocket tcpSocket;
//    FileSessionAdapter *adapter;
    FileTransferManager *manager;

    QLabel *statusBar;
    QLabel *clientStatus;
    QLabel *saveStatus;

private slots:
    void onConfigFileSavePath();
    void onUpdateFileSavePath();
    void onUpdateConnectStatus();

    void onConnect();
    void connected();
    void disconnected();

private:
    QDataStream m_stream;
    qint64 m_fileSize;
    QString m_fileName;
    qint64 m_fileBytesReceivedSize;

    QString savePath;

private:
//    QListWidget *listWidget;
    FileReceiverView *filelistView;

    QComboBox *remoteSwitchBox;
//    QLineEdit *remoteEdit;
    QPushButton *connectBtn;
    QPushButton *saveFileBtn;
    QProgressBar *scanProgressBar;

    QPushButton *receiveFileBtn;
    QProgressBar *currentProgressBar;
    QProgressBar *totalProgressBar;

private:
    ScanWorkerPool *pool;

private:
    void createFileTransferReceiver();
    void scanLocalHost();

private slots:
    void onUseRemoteSwitch();
    void onScanAvilable(QString host, int port);
    void onScanFinished();
    void onScanThreadChanged();

//    void onDownloadFile(const QString filename);
    void onDownloadFileItemInfo(const FileItemInfo &fileinfo);

};

#endif // FILETRANSFERRECEIVER_H
