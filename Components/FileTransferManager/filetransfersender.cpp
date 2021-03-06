#include "filetransfersender.h"

#include <QApplication>
#include <QDragEnterEvent>
#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QMimeData>
#include <QNetworkInterface>
#include <QPushButton>
#include <QStandardPaths>
#include <QStyleFactory>
#include <QTcpSocket>
#include <QTimer>
#include <QVBoxLayout>

FileTransferSender::FileTransferSender(QWidget *parent) : QWidget(parent)
  ,fileListWidget(new QListWidget)
  ,m_readBlock(4096)
  ,m_currentFileSize(0)
  ,m_totalFileSize(0)
  ,m_currentFileBytesWritten(0)
  ,m_totalFileBytesWritten(0)
  ,addFileBtn(new QPushButton("添加"))
  ,delFileBtn(new QPushButton("删除"))
  ,clrFileBtn(new QPushButton("清空"))
  ,sendFileBtn(new QPushButton("发送"))
  ,currentLab(new QLabel("当前进度:"))
  ,totalLab(new QLabel("全部进度:"))
  ,listenPort(new QLabel("....."))
  ,listenStatus(new QLabel("....."))
  ,clientStatus(new QLabel("客户端连接数: 0"))
  ,filesQueueStatus(new QLabel("文件传输队列: 0"))
{
//    qApp->setStyle(QStyleFactory::create("fusion"));

    QRegExp rxIp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    QRegExpValidator v(rxIp);

    if (tcpServer.listen(QHostAddress::Any, 8888)) {
        listenStatus->setText(QStringLiteral("状态：正在监听"));
    } else {
        listenStatus->setText(QStringLiteral("状态：监听失败"));
    }
    listenPort->setText(QString::number(tcpServer.serverPort()));

    createFileTransferSender();

    QList<QHostAddress> localAllAddresses = QNetworkInterface::allAddresses();
    foreach(QHostAddress address, localAllAddresses){
        QString ipAddr = address.toString();
        int pos = 0;
        // use the first IP address
        if (v.validate(ipAddr,pos) == QRegExpValidator::Acceptable){
            listenStatus->setText(listenStatus->text() +" - "+ address.toString());
        }
    }

    setAcceptDrops(true);

    connect(&tcpServer, &QTcpServer::newConnection, this, &FileTransferSender::onNewConnection);

    connect(addFileBtn, &QPushButton::clicked,this,&FileTransferSender::addFile);
    connect(delFileBtn, &QPushButton::clicked,this,&FileTransferSender::delFile);
    connect(clrFileBtn, &QPushButton::clicked,this,&FileTransferSender::clrFile);
    connect(sendFileBtn, &QPushButton::clicked,this,&FileTransferSender::sendFile);

    connect(this, &FileTransferSender::filesAppended, this, &FileTransferSender::onfilesAppended);
    connect(this, &FileTransferSender::filesDeleted, this, &FileTransferSender::onfilesDeleted);
    connect(this, &FileTransferSender::filesCleanded, this, &FileTransferSender::onfilesCleanded);
    connect(this, &FileTransferSender::filesChanged, this, &FileTransferSender::onFileListChanged);
    connect(this, &FileTransferSender::emitFilesQueueChange, this, &FileTransferSender::onFilesQueueChange);

    connect(this, &FileTransferSender::clientChanged,this,&FileTransferSender::onClientChanged);

    setFixedSize(500,500);
}

void FileTransferSender::onNewConnection()
{
    QTcpSocket *localNextPendingConnection = tcpServer.nextPendingConnection();
//    localNextPendingConnection
    m_tcpMap.insert(QString::number(conn_cnt++), localNextPendingConnection);
    connect(localNextPendingConnection,&QTcpSocket::bytesWritten, this, &FileTransferSender::onBytesWritten);
    connect(localNextPendingConnection, &QTcpSocket::readyRead, this, &FileTransferSender::onReadyRead);
    connect(localNextPendingConnection, &QTcpSocket::disconnected, this, &FileTransferSender::onDisconnected);
    emit clientChanged();
//    connect(localNextPendingConnection, &QTcpSocket::disconnected, localNextPendingConnection, &QObject::deleteLater);
}

void FileTransferSender::onReadyRead()
{

}

void FileTransferSender::onDisconnected()
{
    foreach (QString key, m_tcpMap.keys()) {
        QTcpSocket *socket = m_tcpMap[key];
        if (socket->state() == QTcpSocket::UnconnectedState) {
            m_tcpMap.remove(key);
            socket->deleteLater();
        }
    }
    emit clientChanged();
}

void FileTransferSender::onClientChanged()
{
    clientStatus->setText(QString("客户端连接数: %1").arg(m_tcpMap.count()));
}

void FileTransferSender::addFile()
{
    QStringList openFileNames = QFileDialog::getOpenFileNames(this, QStringLiteral("选择文件"),
                                  QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    if (openFileNames.isEmpty()) {
        return;
    }

    QStringList files;

    foreach(QString file, openFileNames){
        if(!compareQListWidgetItem(file)) {
            files << file;
        }
    }

    fileListWidget->addItems(files);

    emit filesAppended(files);
    emit filesChanged();
}

void FileTransferSender::delFile()
{
    int row = fileListWidget->currentRow();
    if (row < 0) {
        return;
    }
    QListWidgetItem *localTakeItem = fileListWidget->takeItem(row);
    emit filesDeleted(QString(localTakeItem->text()));
    emit filesChanged();
    delete localTakeItem;
}

void FileTransferSender::clrFile()
{
    int count = fileListWidget->count();
    if (0 == count) {
        return;
    }
    int ret = QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("确定清空列表吗?"),
                                       QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);
    if (QMessageBox::Ok != ret) {
        return;
    }

    for (int i = 0; i < count; i++) {
        delete fileListWidget->takeItem(0);
    }
    emit filesCleanded();
    emit filesChanged();
}

void FileTransferSender::sendFile()
{
    if (fileListWidget->count() == 0) {
        return;
    }

    if (m_fileQueue.isEmpty()) {
        for (int i = 0; i < fileListWidget->count(); i++) {
            m_fileQueue.append(fileListWidget->item(i)->text());
            m_totalFileSize += QFileInfo(fileListWidget->item(i)->text()).size();
            emit emitFilesQueueChange();
        }
        m_currentFileBytesWritten = 0;
        m_totalFileBytesWritten = 0;
    }

    send();
}

void FileTransferSender::onfilesAppended(QStringList &files)
{
    m_fileQueue.append(files);
    foreach(QString file, files) {
        QFileInfo info(file);
        m_totalFileSize += info.size();
    }
    emit emitFilesQueueChange();
}

void FileTransferSender::onfilesDeleted(QString file)
{
    if (m_fileQueue.contains(file)) {
        m_fileQueue.removeOne(file);
        QFileInfo info(file);
        m_totalFileSize -= info.size();
    }
    emit emitFilesQueueChange();
}

void FileTransferSender::onfilesCleanded()
{
    m_fileQueue.clear();
    emit emitFilesQueueChange();
}

void FileTransferSender::onFileListChanged()
{
//    if (fileListWidget->count() > 0) {
//        currentProgressBar.setFormat(QStringLiteral("%1 : %p%").arg(fileListWidget->item(0)->text()));
//    }
//    if (fileListWidget->count() == 0) {
//        currentProgressBar.setFormat(QStringLiteral("%p%"));
//    }
}

void FileTransferSender::onFilesQueueChange()
{
    filesQueueStatus->setText(QString("文件传输队列: %1").arg(m_fileQueue.count()));
}

void FileTransferSender::onBytesWritten(const qint64 &bytes)
{
    Q_UNUSED(bytes);

    QByteArray array = m_file.read(m_readBlock);
    if (array.isEmpty()) {
        reset();
        return;
    }

    int size = array.size();
    m_stream.writeRawData(array.constData(), size);
    updateProgressBar(size);
}

void FileTransferSender::send()
{
    if (m_tcpMap.isEmpty()){
        return;
    }

    QString localDequeue = m_fileQueue.takeFirst();
    QFileInfo info(localDequeue);

    m_file.setFileName(localDequeue);
    if (!m_file.open(QIODevice::ReadOnly)) {
        return;
    }

    m_currentFileSize = m_file.size();
    currentProgressBar.setFormat(QStringLiteral("%1 : %p%").arg(m_file.fileName()));

    m_stream.setDevice(m_tcpMap.first());
    m_stream.setVersion(QDataStream::Qt_5_0);
    m_stream << m_file.size() << info.fileName();

    QByteArray array = m_file.read(m_readBlock);
    int size = array.size();

    m_stream.writeRawData(array.constData(), size);
    sendFileBtn->setEnabled(false);
    updateProgressBar(size);
    emit emitFilesQueueChange();
}

void FileTransferSender::reset()
{
    m_file.close();
    sendFileBtn->setEnabled(true);

    m_currentFileBytesWritten = 0;
    if (m_fileQueue.isEmpty()) {
        m_totalFileSize = 0;
        m_totalFileBytesWritten = 0;
    } else {
        send();
    }
}

void FileTransferSender::createFileTransferSender()
{
    QGroupBox *senderBox = new QGroupBox("File Sender");
    QHBoxLayout *fileCtlBtnsLayout = new QHBoxLayout;
    QHBoxLayout *currentProgressLayout = new QHBoxLayout;
    QHBoxLayout *totalProgressLayout = new QHBoxLayout;
    QVBoxLayout *senderBoxLayout = new QVBoxLayout;

    /*************************************/
    QFrame *vline;
    vline = new QFrame();
    vline->setFrameShape(QFrame::VLine);
    vline->setFrameShadow(QFrame::Sunken);
    /*************************************/

    fileCtlBtnsLayout->addWidget(addFileBtn);
    fileCtlBtnsLayout->addWidget(delFileBtn);
    fileCtlBtnsLayout->addWidget(clrFileBtn);
    fileCtlBtnsLayout->addWidget(vline);
    fileCtlBtnsLayout->addWidget(sendFileBtn);

    senderBoxLayout->addLayout(fileCtlBtnsLayout);
    senderBoxLayout->addWidget(fileListWidget);

    senderBox->setLayout(senderBoxLayout);

    currentProgressLayout->addWidget(currentLab);
    currentProgressLayout->addWidget(&currentProgressBar);
//    currentProgressBar.setMaximum(100);

    totalProgressLayout->addWidget(totalLab);
    totalProgressLayout->addWidget(&totalProgressBar);
//    totalProgressBar.setMaximum(100);

    /*************************************/
    QFrame *line;
    line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    QFrame *line2 = new QFrame();
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);
    /*************************************/

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(senderBox);
    mainLayout->addWidget(line);
    mainLayout->addLayout(currentProgressLayout);
    mainLayout->addLayout(totalProgressLayout);
    mainLayout->addWidget(line2);
    mainLayout->addWidget(listenStatus);
    mainLayout->addWidget(clientStatus);
    mainLayout->addWidget(filesQueueStatus);
    listenStatus->setText(listenStatus->text() + " - " + listenPort->text());
    setLayout(mainLayout);
}

void FileTransferSender::updateProgressBar(int size)
{
    m_currentFileBytesWritten += size;
    currentProgressBar.setValue((double(m_currentFileBytesWritten)/m_currentFileSize)*100);

    m_totalFileBytesWritten += size;
    totalProgressBar.setValue((double(m_totalFileBytesWritten)/m_totalFileSize)*100);
}

bool FileTransferSender::compareQListWidgetItem(QString b)
{
    QStringList list;
    for (int i = 0; i < fileListWidget->count(); i++) {
        list << fileListWidget->item(i)->text();
    }
    return compareQStringList(list, b);
}

void FileTransferSender::dragMoveEvent(QDragMoveEvent *e)
{
    this->activateWindow();
}

void FileTransferSender::dragEnterEvent(QDragEnterEvent *e)
{
    this->activateWindow();

    const QMimeData *mine = e->mimeData();
    if (!mine->hasUrls()) return e->ignore();

    e->accept();
}

void FileTransferSender::dropEvent(QDropEvent *e)
{
    const QMimeData *mine = e->mimeData();

    QStringList files;
    foreach (QUrl url, mine->urls()) {
        QString localFile = url.toLocalFile();
        QFileInfo info(localFile);

        if (info.isFile()) {
            if(!compareQListWidgetItem(localFile)) {
                files << localFile;
            }
        }
    }
    fileListWidget->addItems(files);
    emit filesAppended(files);
    emit filesChanged();
}
