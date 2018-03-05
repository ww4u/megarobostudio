#ifndef MCSTREAM_H
#define MCSTREAM_H

#include <QtCore>

class ImcStream : public QTextStream
{
public:
    ImcStream();
    ImcStream(QIODevice *device);
    ImcStream(FILE *fileHandle, QIODevice::OpenMode openMode = QIODevice::ReadWrite);
    ImcStream(QString *string, QIODevice::OpenMode openMode = QIODevice::ReadWrite);
    ImcStream(QByteArray *array, QIODevice::OpenMode openMode = QIODevice::ReadWrite);
    ImcStream(const QByteArray &array, QIODevice::OpenMode openMode = QIODevice::ReadOnly);

};

#endif // ISTREAM_H
