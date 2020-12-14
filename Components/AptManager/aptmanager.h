#ifndef SEARCHPACKAGE_H
#define SEARCHPACKAGE_H

#include <QWidget>

#include <QApt/Backend>
//#include <QApt/Transaction>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QLabel;
class QTableWidget;

class AptUtils;
class PackageView;
QT_END_NAMESPACE



class AptManager : public QWidget
{
    Q_OBJECT
public:
    explicit AptManager(QWidget *parent = nullptr);

signals:

public slots:
    void onSearchPackage();

private:
    void initUi();
    QLineEdit *mSearchLine;
    QPushButton *mSearchButton;

    PackageView *m_packageView;

    QLabel *m_packageStatusBarLabel;

    QApt::Backend *m_backend;
};

#endif // SEARCHPACKAGE_H
