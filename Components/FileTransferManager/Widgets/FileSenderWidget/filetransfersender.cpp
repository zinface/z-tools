
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
#include <filesendermodel.h>
#include <filesenderview.h>
#include <QtMath>
#include <filetransfermanager.h>

FileTransferSender::FileTransferSender(QWidget *parent) : QWidget(parent)
  ,manager(new FileTransferManager)
  ,fileListView(new FileSenderView)
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
  ,currentSendSpeed(new QLabel("当前传输速度：0.00 Kb/s"))
{
//    qApp->setStyle(QStyleFactory::create("fusion"));

    QRegExp rxIp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    QRegExpValidator v(rxIp);

    manager->setManagerTask(QString(), 8888);

    if (manager->state()) {
        listenStatus->setText(QStringLiteral("状态：正在监听"));
    } else {
        listenStatus->setText(QStringLiteral("状态：监听失败"));
    }
    listenPort->setText(QString::number(8888));

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

    connect(addFileBtn, &QPushButton::clicked,this,&FileTransferSender::addFile);
    connect(delFileBtn, &QPushButton::clicked,this,&FileTransferSender::delFile);
    connect(clrFileBtn, &QPushButton::clicked,this,&FileTransferSender::clrFile);
    connect(sendFileBtn, &QPushButton::clicked,this,&FileTransferSender::sendFile);

    connect(this, &FileTransferSender::filesAppended, this, &FileTransferSender::onfilesAppended);
    connect(this, &FileTransferSender::filesDeleted, this, &FileTransferSender::onfilesDeleted);
    connect(this, &FileTransferSender::filesCleanded, this, &FileTransferSender::onfilesCleanded);

    connect(this, &FileTransferSender::filesAppended, fileListView, &FileSenderView::appendFilse);
    connect(this, &FileTransferSender::filesDeleted, fileListView, &FileSenderView::filesDeleted);
    connect(this, &FileTransferSender::filesCleanded, fileListView, &FileSenderView::filesCleanded);

    connect(this, &FileTransferSender::filesChanged, this, &FileTransferSender::onFileListChanged);

    connect(this, &FileTransferSender::emitFilesQueueChange, this, &FileTransferSender::onFilesQueueChange);

    connect(manager, &FileTransferManager::onRemoteFetchFileList, this, &FileTransferSender::onClientFetchFileList);
    connect(manager, &FileTransferManager::onRemoteFetchFile, this, &FileTransferSender::onClientFetchFile);
    connect(manager, &FileTransferManager::clientCountChanged, this, &FileTransferSender::onClientChanged);

    setFixedSize(500,500);
}

FileTransferSender::~FileTransferSender()
{

}

void FileTransferSender::onClientChanged(int count)
{
    clientStatus->setText(QString("客户端连接数: %1").arg(count));
}

void FileTransferSender::onClientFetchFileList(QTcpSocket *c)
{
    for (int i=0; i<fileListView->count(); i++) {
        manager->broadCaseAction(c, FileTransferManager::S_APPEND, fileListView->item(i)->fileName, fileListView->item(i)->filesize);
    }
}

void FileTransferSender::onClientFetchFile(QTcpSocket *c, QString filename)
{
    for (int i=0; i<fileListView->count(); i++) {
       if (fileListView->item(i)->fileName == filename) {
           manager->broadCaseAction(c, FileTransferManager::OP_UPLOAD, fileListView->item(i)->fileName, fileListView->item(i)->filesize, fileListView->item(i)->filePath);
           return;
       }
    }
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

    emit filesAppended(files);
    emit filesChanged();
}

void FileTransferSender::delFile()
{
    int row = fileListView->currentRow;
    if (row < 0 || row >= fileListView->count()) {
        return;
    }
    emit filesDeleted(fileListView->item(row)->filePath);
    emit filesChanged();
}

void FileTransferSender::clrFile()
{
    int count = fileListView->count();
    if (0 == count) {
        return;
    }
    int ret = QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("确定清空列表吗?"),
                                       QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);
    if (QMessageBox::Ok != ret) {
        return;
    }

    emit filesCleanded();
    emit filesChanged();
}

void FileTransferSender::sendFile()
{
//    if (fileListView->count() == 0) {
//        return;
//    }
//    timer.restart();
//    if (m_fileQueue.isEmpty()) {
//        for (int i = 0; i < fileListView->count(); i++) {
//            m_fileQueue.append(fileListView->item(i)->filePath);
//            m_totalFileSize += QFileInfo(fileListView->item(i)->filePath).size();
//            emit emitFilesQueueChange();
//        }
//        m_currentFileBytesWritten = 0;
//        m_totalFileBytesWritten = 0;
//    }

//    send();
}

void FileTransferSender::onfilesAppended(QStringList &filepaths)
{
    m_fileQueue.append(filepaths);
    foreach(QString filepath, filepaths) {
        QFileInfo info(filepath);
        m_totalFileSize += info.size();
        manager->pushFileAppend(info.fileName(), info.size());
    }
    emit emitFilesQueueChange();
}

void FileTransferSender::onfilesDeleted(QString filepath)
{
    QString filename;
    if (m_fileQueue.contains(filepath)) {
        m_fileQueue.removeOne(filepath);
        QFileInfo info(filepath);
        filename = info.fileName();
        m_totalFileSize -= info.size();
    }
    manager->pushFileDeleted(filename);
    emit emitFilesQueueChange();
}

void FileTransferSender::onfilesCleanded()
{
    m_fileQueue.clear();
    manager->pushFileClaer();
    emit emitFilesQueueChange();
}

void FileTransferSender::onFileListChanged()
{
    if (fileListView->count() > 0) {
        currentProgressBar.setFormat(QStringLiteral("%1 : %p%").arg(fileListView->item(0)->filePath));
    }
    if (fileListView->count() == 0) {
        currentProgressBar.setFormat(QStringLiteral("%p%"));
    }
    fileListView->getModel()->layoutChanged();
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
//    if (m_tcpMap.isEmpty()){
//        return;
//    }

    QString localDequeue = m_fileQueue.takeFirst();
    QFileInfo info(localDequeue);

    m_file.setFileName(localDequeue);
    if (!m_file.open(QIODevice::ReadOnly)) {
        return;
    }

    m_currentFileSize = m_file.size();
    currentProgressBar.setFormat(QStringLiteral("%1 : %p%").arg(m_file.fileName()));

//    m_stream.setDevice(m_tcpMap.first());
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
    sendFileBtn->setEnabled(false);

    senderBoxLayout->addLayout(fileCtlBtnsLayout);
//    senderBoxLayout->addWidget(fileListWidget);
    senderBoxLayout->addWidget(fileListView);

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
//    mainLayout->addLayout(currentProgressLayout);
//    mainLayout->addLayout(totalProgressLayout);
//    mainLayout->addWidget(line2);
    mainLayout->addWidget(listenStatus);
    mainLayout->addWidget(clientStatus);
    QHBoxLayout *bottomWidgetsLayout = new QHBoxLayout;
    bottomWidgetsLayout->addWidget(filesQueueStatus, Qt::AlignLeft | Qt::AlignVCenter);

    QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
//    sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());

    currentSendSpeed->setSizePolicy(sizePolicy);
    bottomWidgetsLayout->addWidget(currentSendSpeed, Qt::AlignRight | Qt::AlignVCenter);
    mainLayout->addLayout(bottomWidgetsLayout);
//    mainLayout->addWidget(filesQueueStatus);
//    mainLayout->addWidget(currentSendSpeed);
    listenStatus->setText(listenStatus->text() + " - " + listenPort->text());
    setLayout(mainLayout);
}

void FileTransferSender::updateProgressBar(int size)
{
    m_currentFileBytesWritten += size;
    currentProgressBar.setValue((double(m_currentFileBytesWritten)/m_currentFileSize)*100);

    m_totalFileBytesWritten += size;
    totalProgressBar.setValue((double(m_totalFileBytesWritten)/m_totalFileSize)*100);

    // 当前传输速度：0.00 Kb/s
    double speed = (double)m_totalFileBytesWritten/timer.elapsed();
    currentSendSpeed->setText(QString("当前传输速度：%1 %2").arg(QString::number(speed/ (1024*1024/1000),'f',3)).arg("Mb/s"));
}

bool FileTransferSender::compareQListWidgetItem(QString b)
{
    QStringList list;
    for (int i = 0; i < fileListView->count(); i++) {
        list << fileListView->item(i)->filePath;
    }
    return compareQStringList(list, b);
}

void FileTransferSender::dragMoveEvent(QDragMoveEvent *e)
{
    Q_UNUSED(e);
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
    emit filesAppended(files);
    emit filesChanged();
}
