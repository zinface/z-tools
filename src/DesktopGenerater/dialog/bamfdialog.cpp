#include "bamfdialog.h"
#include "ui_bamfdialog.h"

#include <QIcon>
#include <matcher.h>
#include <view.h>
#include <window.h>

Application::Application()
{
}

QString Application::name() const
{
    return m_name;
}

void Application::setName(const QString &newName)
{
    m_name = newName;
}

QString Application::icon() const
{
    return m_icon;
}

void Application::setIcon(const QString &newIcon)
{
    m_icon = newIcon;
}

QString Application::symLink() const
{
    return m_symLink;
}

void Application::setSymLink(const QString &newSymLink)
{
    m_symLink = newSymLink;
}


BamfDialog::BamfDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BamfDialog)
{
    ui->setupUi(this);


    loadApplications();
}

BamfDialog::~BamfDialog()
{
    delete ui;
}

void BamfDialog::loadApplications()
{
    Matcher matcher("org.ayatana.bamf", "/org/ayatana/bamf/matcher", QDBusConnection::sessionBus());
    auto paths = matcher.WindowPaths();
    paths.waitForFinished();

    if (paths.isValid()) {
        // qDebug() << paths.value();
        foreach  (auto path, paths.value()) {
            qDebug() << "Window:" << path;
            Window window("org.ayatana.bamf", path, QDBusConnection::sessionBus());
            auto pid = window.GetPid();
            pid.waitForFinished();
            QFileInfo file(QString("/proc/%1/exe").arg(pid));
            qDebug() << "PID:" << pid.value() << file.symLinkTarget();

            View view("org.ayatana.bamf", path, QDBusConnection::sessionBus());
            auto name = view.name();
            // name.waitForFinished();
            // qDebug() << "Name:" << name;

            auto parents = view.Parents();
            parents.waitForFinished();

            // qDebug() << "Parents:" << parents.value() << "," << "Count:" << parents.value().count();

            if (parents.value().count() > 0) {
                View parent("org.ayatana.bamf", parents.value().first(), QDBusConnection::sessionBus());
                auto parentName = parent.name();
                auto parentIcon = parent.icon();
                auto parentPath = parent.path();

                QIcon icon_image;
                if (QFileInfo::exists(parentIcon)) {
                    icon_image = QIcon(parentIcon);
                } else {
                    icon_image = QIcon::fromTheme(parentIcon);
                }

                QListWidgetItem *item = new QListWidgetItem;
                item->setIcon(icon_image);
                item->setText(parentName);

                item->setData(990, parentName);
                item->setData(991, parentIcon);
                item->setData(992, file.symLinkTarget());
                ui->listWidget->addItem(item);

                qDebug() << "Name:" << name << "," << "Parent:" << parentName << "," << "Icon:" << parentIcon << "," << "Path:" << parentPath;
            }
        }
    }
}

Application BamfDialog::getSelectApplication()
{
    auto item = ui->listWidget->currentItem();

    Application app;
    app.setName(item->data(990).toString());
    app.setIcon(item->data(991).toString());
    app.setSymLink(item->data(992).toString());

    return app;
}

void BamfDialog::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (current)
    emit onApplicationSelected(getSelectApplication());
}
