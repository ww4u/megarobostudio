#include "imcstream.h"

ImcStream::ImcStream()
{

}

ImcStream::ImcStream(QIODevice *device) : QTextStream(device)
{}
ImcStream::ImcStream(FILE *fileHandle, QIODevice::OpenMode openMode )
          : QTextStream( fileHandle, openMode )
{}
ImcStream::ImcStream(QString *string, QIODevice::OpenMode openMode)
          : QTextStream( string, openMode )
{}
ImcStream::ImcStream(QByteArray *array, QIODevice::OpenMode openMode)
          : QTextStream( array, openMode )
{}
ImcStream::ImcStream(const QByteArray &array, QIODevice::OpenMode openMode)
          : QTextStream( array, openMode )
{}
