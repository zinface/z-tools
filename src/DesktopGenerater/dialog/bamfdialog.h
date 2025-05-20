#ifndef BAMFDIALOG_H
#define BAMFDIALOG_H

#include <QDialog>


class QListWidgetItem;
namespace Ui {
class BamfDialog;
}

class Application {
public:
    Application();
    QString name() const;
    void setName(const QString &newName);
    QString icon() const;
    void setIcon(const QString &newIcon);
    QString symLink() const;
    void setSymLink(const QString &newSymLink);

private:
    QString m_name;
    QString m_icon;
    QString m_symLink;
};
Q_DECLARE_METATYPE(Application)

class BamfDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BamfDialog(QWidget *parent = nullptr);
    ~BamfDialog();

    void loadApplications();

    Application getSelectApplication();

private slots:
    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

signals:
    void onApplicationSelected(Application app);

private:
    Ui::BamfDialog *ui;
};

#endif // BAMFDIALOG_H
