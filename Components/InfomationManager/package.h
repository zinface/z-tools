#ifndef PACKAGE_H
#define PACKAGE_H

#include <QByteArray>
#include <QString>

class Package
{
public:
    Package() {
        buffer.resize(0);
    }
    Package(QByteArray &data) {
        buffer.append(data);
    }
    ~Package() {
        buffer.clear();
    };

    static QByteArray lenf(int length, int size){
        QByteArray buf;
        buf.append(QByteArray::number(length));
        buf.resize(size);
        return buf;
    };

    void setByteArrayData(QByteArray &data) {
        buffer.append(data);
    }

    QByteArray &toByteArray() {
        return buffer;
    }

    Package &operator<<(qint8 number) {
        int space = sizeof(qint8);
        QByteArray temp = QByteArray::number(number);
        buffer.append(lenf(temp.length(),space));
        buffer.append(temp); return *this;
    }
    Package &operator>>(qint8 &number) {
        int space = sizeof(qint8);
        int length = buffer.length();
        int vlen = buffer.left(space).toInt();
        buffer = buffer.right(length-space);
        number = buffer.left(vlen).toUShort();
        buffer = buffer.right(length-vlen-space);
        return *this;
    }

    Package &operator<<(qint16 number) {
        int space = sizeof(qint16);
        QByteArray temp = QByteArray::number(number);
        buffer.append(lenf(temp.length(),space));
        buffer.append(temp); return *this;
    }
    Package &operator>>(qint16 &number) {
        int space = sizeof(qint16);
        int length = buffer.length();
        int vlen = buffer.left(space).toLongLong();
        buffer = buffer.right(length-space);
        number = buffer.left(vlen).toShort();
        buffer = buffer.right(length-vlen-space);
        return *this;
    }

    Package &operator<<(qint32 number) {
        int space = sizeof(qint32);
        QByteArray temp = QByteArray::number(number);
        buffer.append(lenf(temp.length(),space));
        buffer.append(temp); return *this;
    }
    Package &operator>>(qint32 &number) {
        int space = sizeof(qint32);
        int length = buffer.length();
        int vlen = buffer.left(space).toInt();
        buffer = buffer.right(length-space);
        number = buffer.left(vlen).toUInt();
        buffer = buffer.right(length-vlen-space);
        return *this;
    }

    Package &operator<<(qint64 number) {
        int space = sizeof(qint64);
        QByteArray temp = QByteArray::number(number);
        buffer.append(lenf(temp.length(),space));
        buffer.append(temp); return *this;
    }
    Package &operator>>(qint64 &number) {
        int space = sizeof(qint64);
        int length = buffer.length();
        int vlen = buffer.left(space).toInt();
        buffer = buffer.right(length-space);
        number = buffer.left(vlen).toLongLong();
        buffer = buffer.right(length-vlen-space);
        return *this;
    }

    Package &operator<<(QString value) {
        buffer.append(lenf(value.toLocal8Bit().length(), 8));
        buffer.append(value.toLocal8Bit());return *this;
    }
    Package &operator>>(QString &value) {
        int length = buffer.length();
        int vlen = buffer.left(8).toInt();
        buffer = buffer.right(length-8);
        value = QString(buffer.left(vlen));
        buffer = buffer.right(length-vlen-8);
        return *this;
    }
    Package &operator<<(Package package) {
        buffer.append(package.toByteArray());
        return *this;
    }
    int size() {
        return buffer.length();
    }

private:
    QByteArray buffer;
};

#endif // PACKAGE_H
