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
class QTableWidget;
class QComboBox;

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
    void onPackageChange();
    void onCategoryChange();
    void onArchCategoryChange();

private:
    /******* 类别控制 *******/
    QLabel *m_packageCategoryLab;
    QComboBox *m_packageArchCategory;
    QComboBox *m_packageCategory;

private:
    /******** 搜索框 和 按钮 ******/
    QLabel *mSearchLab;
    QLineEdit *mSearchEdit;
    QPushButton *mSearchBtn;

private:
    /******* 结果展示Widget ******/
    PackageView *m_packageView;

private:
    QLabel *m_statusBar;

private:
    void createAptManager();
    AptUtils mAptUtil;
};

#endif // SEARCHPACKAGE_H
