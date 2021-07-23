#ifndef FILETRANSFERRECIVERMANUALHOSTDIALOG_H
#define FILETRANSFERRECIVERMANUALHOSTDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QMutex>

class QLabel;
class QLineEdit;
class ScanWorkerPool;

class FileTransferReciverManualHostDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FileTransferReciverManualHostDialog(QWidget *parent = nullptr);

private:
    void createFileTransferReciverManualHostDialog();


public:
    QString getHost();

private:
    QLabel *ipLabel;
    QLineEdit *hostEdit;
    QLabel *hostTestLabel;
    QPushButton *confirmBtn;
    QPushButton *testBtn;

    ScanWorkerPool *pool;

private:
    bool connected = false;
    QMutex mute;

private slots:
    void onTestConnected();
    void onTestFinished();
    void onTestBtnChoose();
    void onConfirmChoose();

};

#endif // FILETRANSFERRECIVERMANUALHOSTDIALOG_H
