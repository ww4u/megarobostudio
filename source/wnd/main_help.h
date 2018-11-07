#ifndef MAIN_HELP_H
#define MAIN_HELP_H

#include <QtCore>
#include <QApplication>

class CommonHelper
{
public:
    static void setStyle(const QString &style) {
        QFile qss(style);
        qss.open(QFile::ReadOnly);
        qApp->setStyleSheet(qss.readAll());
        qss.close();
    }

    static void selectStyle( int index )
    {
        if ( index == 0 )
        {
            CommonHelper::setStyle( qApp->applicationDirPath() + "/style" + "/classic.qss" );

            QResource::registerResource( qApp->applicationDirPath() + "/style" + "/black.rcc" );
        }
        else
        {
            CommonHelper::setStyle( qApp->applicationDirPath() + "/style" + "/mega.qss" );

            QResource::registerResource( qApp->applicationDirPath() + "/style" + "/res.rcc" );
        }
    }
};


#endif // MAIN_HELP_H
