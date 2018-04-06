#include <QApplication>
#include <QDebug>

#include "../../include/mcstd.h"

#include "../../com/comassist.h"
int main(int argc, char *argv[] )
{
    QApplication app( argc, argv );

//    QString strFile( "G:\\work\\mc\\develope\\source\\wnd\\build-mc-mingw-Debug\\train_session\\Delta_.dsp" );
    QString strFile( "G:\\work\\mc\\develope\\installer\\package\\dataset\\a.txt" );
    QList<float> dataSet;
    comAssist::loadDataset( strFile,
                            8,
                            dataSet );
    logDbg()<<dataSet.size();
    foreach( float f, dataSet )
    {
        logDbg()<<f;
    }

    return 0;
}
