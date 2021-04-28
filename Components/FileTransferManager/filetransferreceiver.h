#ifndef FILETRANSFERRECEIVER_H
#define FILETRANSFERRECEIVER_H

#include <QFile>
#include <QTcpSocket>
#include <QWidget>

class QComboBox;
class QLabel;
class QLineEdit;
class QListWidget;
class QProgressBar;
class QPushButton;
class ScanWorkerPool;

class FileTransferReceiver : public QWidget
{
    Q_OBJECT
public:
    explicit FileTransferReceiver(QWidget *parent = nullptr);


private:
    QTcpSocket tcpSocket;

    QLabel *statusBar;
    QLabel *clientStatus;

private slots:
    void onConfigFileSavePath();
    void onUpdateFileSavePath();

    void onConnect();
    void connected();
    void disconnected();

private slots:
    void onReadyRead();

private:
    QDataStream m_stream;
    qint64 m_fileSize;
    QString m_fileName;
    QFile m_file;
    qint64 m_fileBytesReceivedSize;

    QString savePath;

private:
    QListWidget *listWidget;

    QComboBox *remoteSwitchBox;
    QLineEdit *remoteEdit;
    QPushButton *connectBtn;
    QPushButton *saveFileBtn;
    QProgressBar *scanProgressBar;

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

};

#endif // FILETRANSFERRECEIVER_H
