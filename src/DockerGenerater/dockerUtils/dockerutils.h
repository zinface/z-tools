#ifndef DOCKERUTILS_H
#define DOCKERUTILS_H

#include <QObject>

class DockerUtils : public QObject
{
    Q_OBJECT
public:
    explicit DockerUtils(QObject *parent = nullptr);

signals:

public slots:
};

#endif // DOCKERUTILS_H