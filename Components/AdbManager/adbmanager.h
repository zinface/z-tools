#ifndef ADBMANAGER_H
#define ADBMANAGER_H

#include <QWidget>

class QGroupBox;
class QLabel;
class QComboBox;
class QPushButton;
class AdbUtils;
class AdbPkgView;

class AdbManager : public QWidget
{
    Q_OBJECT
public:
    explicit AdbManager(QWidget *parent = nullptr);

public slots:
    void refreshDevices();
    void setDeviceInfomation(int index);
    void setPackageMgtag(int index);

private:
    QGroupBox *adbGroupBox;
    QLabel *adbDeviceLabel;
    QComboBox *deviceComb;
    QLabel *adbBrandLabel;
    QLabel *deviceBrand;
    QLabel *adbModelLabel;
    QLabel *deviceModel;
    QLabel *adbVersionLabel;
    QLabel *deviceVersion;
    QLabel *adbPhysicalLabel;
    QLabel *devicePhysical;
    QPushButton *refresh;

    QGroupBox *adbPackageGroupBox;
    QLabel *m_packageTagLabel;
    QComboBox *m_packageTagComb;
    AdbPkgView *m_AdbPkgView;

    void createAdbInfomation();
    void createAdbPackageLayout();

    AdbUtils *adbutils;

    QString m_currentDevice;
};

#endif // ADBMANAGER_H
