#ifndef FILETRANSFERRECEIVER_H
#define FILETRANSFERRECEIVER_H

#include <QFile>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QTcpSocket>
#include <QWidget>

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

    QLineEdit *remoteEdit;
    QPushButton *connectBtn;
    QPushButton *saveFileBtn;


private:
    void createFileTransferReceiver();

};

#endif // FILETRANSFERRECEIVER_H
