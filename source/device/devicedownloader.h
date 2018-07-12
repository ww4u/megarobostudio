#ifndef DEVICEDOWNLOADER_H
#define DEVICEDOWNLOADER_H

#include <QtCore>

class DeviceDownloader : public QThread
{
    Q_OBJECT
protected:
    static QList<DeviceDownloader*> _activeLoaders;
    static QMutex _activeLoadersMutex;

public:
    static void cancelActives();

protected:
    static void append( DeviceDownloader *pLoader );
    static void remove( DeviceDownloader *pLoader );

public:
    explicit DeviceDownloader(QObject *parent = nullptr);
    virtual ~DeviceDownloader();

signals:

public slots:
};

#endif // DEVICEDOWNLOADER_H
