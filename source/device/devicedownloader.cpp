#include "devicedownloader.h"

QList<DeviceDownloader*> DeviceDownloader::_activeLoaders;
QMutex DeviceDownloader::_activeLoadersMutex;

void DeviceDownloader::cancelActives()
{
    DeviceDownloader::_activeLoadersMutex.lock();

    foreach( DeviceDownloader*pLoader,
             DeviceDownloader::_activeLoaders )
    {
        Q_ASSERT( pLoader != NULL );
        pLoader->requestInterruption();
    }

    DeviceDownloader::_activeLoadersMutex.unlock();
}

void DeviceDownloader::append( DeviceDownloader *pLoader )
{
    Q_ASSERT( NULL != pLoader );
    DeviceDownloader::_activeLoadersMutex.lock();

    DeviceDownloader::_activeLoaders.append( pLoader );

    DeviceDownloader::_activeLoadersMutex.unlock();
}
void DeviceDownloader::remove( DeviceDownloader *pLoader )
{
    Q_ASSERT( NULL != pLoader );
    DeviceDownloader::_activeLoadersMutex.lock();

    DeviceDownloader::_activeLoaders.removeAll( pLoader );

    DeviceDownloader::_activeLoadersMutex.unlock();
}

DeviceDownloader::DeviceDownloader(QObject *parent) : QThread(parent)
{
    DeviceDownloader::append( this );
}

DeviceDownloader::~DeviceDownloader()
{
    DeviceDownloader::remove( this );
}
