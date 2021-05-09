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
        UPLOADING,
        UPLOADED
    };

    enum FileDonwloadStat {
        NOT_DOWNLOAD,
        SIZEWARRING,
        DOWNLOADING,
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
        FileReceiverSizeStringRole,
        FileReceiverDownloadRole,
        FileReceiverDownloadSize
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
    qint64 filesize;
    FileUploadStat _upstate;
    FileDonwloadStat _downstate;

    qint64 _totalBytes = 0;

public:
    /* 自动计算值和解出值单位 */
    static QString autoRateUnits(qint64 speed, RateUnit &unit, double &val);
    static QString convertRateUnits(RateUnit &unit);


public slots:
    void onTotalWriteBytes(qint64 bytes);

signals:
    void onBytesChanged();
};

#endif // FILEITEMINFO_H
