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
    QLabel *m_statusBar;
    QCheckBox *m_refreshCheck;

    AptUtils mAptUtil;

signals:
    void packageChange(PackageView *m_packageView);
    

public slots:
    void onPackageChange(PackageView *m_packageView);
};

#endif // SEARCHPACKAGE_H
