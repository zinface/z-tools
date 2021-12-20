#include "directorysender.h"
#include "DirectoryActions.h"

#include <QDir>
#include <QFunctionPointer>

DirectorySender::DirectorySender(QObject *parent) : QObject(parent)
{
    // 需要注册的事件信息：事件动作，数据模式，生命周期接管过滤
    manager.registerAction(DirectoryActions::FetchWorkDirectoryTree);
    manager.registerAction(DirectoryActions::FetchWorkDirectoryFiles);

    connect(&manager, &InfomationManager::onRemoteFetch, this, &DirectorySender::onRemoteFetch);
    connect(&manager, &InfomationManager::onRemoteFetchRaw, this, &DirectorySender::onRemoteFetchRaw);
}

void DirectorySender::start()
{
    manager.setManagerMode("", 9999, SessionManager::SERVER);
}

void DirectorySender::broadCaseWorkDirectoryTree(qint8 action, QTcpSocket *c, QString parent, QString relative)
{
    foreach(QFileInfo info, QDir(parent).entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries)) {
        QString path = QString("%1/%2").arg(relative).arg(info.fileName());
        Package package;
        if (info.isDir()) {
            package << qint8(RAWDIR) << qint64(-1) << path;
            manager.broadCaseAction(c, action, package.toByteArray().length(), package.toByteArray());
            broadCaseWorkDirectoryTree(action, c, QDir(parent).absoluteFilePath(info.fileName()), relative+"/"+info.fileName());
        } else {
            package << qint8(RAWFILE) << qint64(info.size()) << path;
            manager.broadCaseAction(c, action, package.toByteArray().length(), package.toByteArray());
        }
    }
}

void DirectorySender::onRemoteFetchRaw(qint8 action, qint64 length, QByteArray &data, QTcpSocket *c)
{

}

void DirectorySender::onRemoteFetch(qint8 action, QTcpSocket *c)
{
    if(action == FetchWorkDirectoryTree) {
        broadCaseWorkDirectoryTree(action, c, this->_workDir, ".");
    }
}


/**
/Users/zinface/tmp/images
├── Anime-动漫
│   ├── 083385214AA8D10B7358F39108BC6D40.jpg
│   ├── 08B140C6EC7832707282C75BF7972E51.jpg
│   ├── 0A812EE4A3F78E3B55F3528A2EDD6167.png
│   ├── 0B57647738B0E24C4297F4BB4CA325C1.jpg
│   ├── 0D6DBE27B17F98DC0CB91A4AA7CC0962.jpg
│   ├── 1A902E9DEF5EADFA7FC59BAD2498BD5F.jpg
│   ├── 1B633D3C7174B61ACFDF846565B27F78.jpg
│   ├── 3A6324AA86895B0680038F3AF8A8EB64.jpg
│   ├── 3D7122E29EEF277400D865ED8B1F0929.jpg
│   ├── 3F262BE8FB76A961A963EF4F9423FA1C.jpg
│   ├── 5E3DAEC3C2D865A9DA20535CFC730AB1.jpg
│   ├── 5F11D6E7C89412D41463A67A6ECD379A.png
│   ├── 66E512C9832F970B8F41265219788BFC.jpg
│   ├── 83EC32D6499732802171CBC5172C6465.jpg
│   ├── 8A558BA7E5C0991085D40900A5A15484.jpg
│   ├── CD5115D6BD16A7B280F9F4B664565111.jpg
│   └── F4F1CDFCCDE6E4205420599DA5975229.jpg
├── emtpy
├── LICENSE
└── README.md
*/
