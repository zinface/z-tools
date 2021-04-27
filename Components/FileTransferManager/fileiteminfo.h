#ifndef FILEITEMINFO_H
#define FILEITEMINFO_H

#include <QObject>
#include <QString>

class FileItemInfo : public QObject
{
    Q_OBJECT
public:
    FileItemInfo(QObject *parent = 0);
    void setName(const QString &name);
    void setpath(const QString &path);
    void setSize(qint64 size);
    void setFileUploadStat(int state = NOT_UPLOAD);
    void setFileDownloadStat(int state = NOT_DOWNLOAD);

    QString getRateSize();

public:
    enum FileUploadStat {
        NOT_UPLOAD,
        UPLOADED
    };

    enum FileDonwloadStat {
        NOT_DOWNLOAD,
        DOWNLOADED
    };

    enum FileSenderItemRole {
        FileSenderNameRole,
        FileSenderPathRole,
        FileSenderSizeRole,
        FileSenderUploadRole,
    };

    enum FileReceiverItemRole {
        FileReceiverNameRole,
        FileReceiverPathRole,
        FileReceiverSizeRole,
        FileReceiverDownloadRole
    };

    enum RateUnit {
        RateBit,
        RateByte,
        RateKb,
        RateMb,
        RateGb,
        RateTb,
        RateUnknow
    };

public:
    QString fileName;
    QString filePath;
    QString fileSize;
    FileUploadStat _upstate;
    FileDonwloadStat _downstate;

public:
    /* 自动计算值和解出值单位 */
    static QString autoRateUnits(qint64 speed, RateUnit &unit, double &val);
    static QString convertRateUnits(RateUnit &unit);
};

#endif // FILEITEMINFO_H
