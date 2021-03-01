#ifndef FILETRANSFERMANAGER_H
#define FILETRANSFERMANAGER_H

#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextEdit>
#include <QWidget>

class FileTransferManager : public QWidget
{
    Q_OBJECT
public:
    explicit FileTransferManager(QWidget *parent = nullptr);
    ~FileTransferManager();

signals:

public slots:
    void onSendChanged();
    void onNewConnection();
    void ondisConnect();
    void onReadRead();
    void onSocketError(QAbstractSocket::SocketError error);

private:
    QGroupBox *fileSendBox;
    QGroupBox *fileRecevieBox;
    QGroupBox *fileTransferBox;
    QComboBox *sendComb;

    QTextEdit *sendText;
    QTextEdit *receiveText;

    QLabel *clients;

    void createFileTransferManager();

private:
    QTcpServer tss;
    QTcpSocket tsc;
    QTcpSocket *m_cSocket;

    int count = 0;
    QMap<QString, QTcpSocket*> smap;
};

#endif // FILETRANSFERMANAGER_H
