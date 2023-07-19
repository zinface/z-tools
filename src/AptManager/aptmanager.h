#ifndef SEARCHPACKAGE_H
#define SEARCHPACKAGE_H

#include <QWidget>

#include <QApt/Backend>

#include <AptUtils/aptutils.h>
//#include <QApt/Transaction>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QLabel;
class QCheckBox;
class QTableWidget;
class QComboBox;

class PackageView;
QT_END_NAMESPACE

class AptManager : public QWidget
{
    Q_OBJECT
public:
    explicit AptManager(QWidget *parent = nullptr);

private:
    // QLabel    m_packageCategory;
    QComboBox *m_packageArchitectureCategory;
    //QLabel   *m_packageArchitecture;
    QComboBox *m_packageInstallCategory;

    QLineEdit *packageSearchEdit;
    QPushButton *packageSearchButton;

    QLabel *m_statusBar;
    QCheckBox *m_refreshCheck;
    QPushButton *m_updateBtn;

    AptUtils mAptUtil;

signals:
    void packageChange(PackageView *m_packageView);
    

public slots:
    void onPackageChange(PackageView *m_packageView);

    // QObject interface
public:
    bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // SEARCHPACKAGE_H
