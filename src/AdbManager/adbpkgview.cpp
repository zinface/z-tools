#include "adbpkgview.h"
#include "AdbUtils/adbutils.h"
#include <QMenu>
#include <qdebug.h>

AdbPkgView::AdbPkgView(QWidget *parent) : QListView(parent)
//  ,m_data(new QStringList)
  ,m_model(new QStringListModel)
{
    setModel(m_model);

    setContextMenuPolicy(Qt::CustomContextMenu);
    

    // void customContextMenuRequested(const QPoint &pos);
    connect(this, &AdbPkgView::customContextMenuRequested, [&](const QPoint &pos){
        // 这里的 debug 日志是打印简单的信息
        qDebug() << "customContextMenuRequested: "
            << "this->currentIndex(): " << (QModelIndex)this->currentIndex()
            << "this->m_data.length(): " << this->m_data.length();

        // 这里的 debug 日志是打印 QModelIndex 的信息
        auto model_index = (QModelIndex)this->currentIndex();
        qDebug() << "model_index: "
            << "model_index.column(): " << model_index.column()
            << "model_index.row(): " << model_index.row()
            << "model_index.flags(): " << model_index.flags()
            << "model_index.isValid(): " << model_index.isValid();
            
        if (!model_index.isValid()) return;

        // 这里的 debug 日志是打印 model 可用时 的信息
        qDebug() << "model_index.isValid():"
            << "model_index.data().toString():" << model_index.data().toString();
        
        QMenu *menu = new QMenu(this);
        menu->addAction("此功能请谨慎使用")->setEnabled(false);
        menu->addSeparator();
        menu->addAction("禁用", [this](){
            this->disable_pkg(this->currentIndex().data().toString());
        });
        menu->addAction("启用", [this](){
            this->enable_pkg(this->currentIndex().data().toString());
        });
        menu->move(mapToGlobal(pos));
        // menu->show();
        menu->exec();
        menu->deleteLater();
    });
}

void AdbPkgView::appendData(const QString &item)
{
    m_data.append(item);
    m_model->setStringList(m_data);
}

void AdbPkgView::setData(QStringList items)
{
    m_data = items;
    m_model->setStringList(m_data);
}

void AdbPkgView::enable_pkg(QString pkg)
{
    pkg = pkg.split(":")[1];
    QString args = QString("shell pm enable %1").arg(pkg);
    qDebug() << "[CALL]:" << __FUNCTION__ << args << "\n" << 
        AdbUtils::RunCommand(args);
}

void AdbPkgView::disable_pkg(QString pkg) {
    pkg = pkg.split(":")[1];
    QString args = QString("shell pm disable %1").arg(pkg);
    qDebug() << "[CALL]:" << __FUNCTION__ << args << "\n" << 
        AdbUtils::RunCommand(args);
}
