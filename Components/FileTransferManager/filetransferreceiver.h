#ifndef FILETRANSFERRECEIVER_H
#define FILETRANSFERRECEIVER_H

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
    void onConnect();
    void connected();
    void disconnected();


private:
    QListWidget *listWidget;

    QLineEdit *remoteEdit;
    QPushButton *connectBtn;
    QPushButton *saveFileBtn;



private:
    void createFileTransferReceiver();


};

#endif // FILETRANSFERRECEIVER_H
