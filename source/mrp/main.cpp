#include <QCoreApplication>

#include "mdataset.h"
#include "../../include/mcstd.h"

int main( int argc, char **argv )
{
    QCoreApplication app( argc, argv );

    QVariant var;
    var.setValue( QString("f") );

    if ( var.isValid() )
    { logDbg()<<var.toBool(); }
    else
    {}
//    QStringList strList;

//    for ( int i = 0; i < 10; i++ )
//    {
////        strList<<QString("\%%1").arg( i + 1 );
//        strList<<QString("%1");
//    }

//    logDbg()<<strList.join('/');
//    QString fmt;
//    for ( int i = 0; i < 10; i++ )
//    {
//        fmt = QString("%1").arg( strList.join('/') ).arg( i );
//        logDbg()<<fmt;
//    }


//    MDataSet dataSet;

//    dataSet.load("C:/Users/wangzhiyan/Desktop/a.mrp");

//    MDataSection *pSec;
//    pSec = dataSet.section( 0 );
//    QVariant var;
//    for ( int i = 0; i < dataSet.columns(); i++ )
//    {
//        var = pSec->cell( 0, i );
//        logDbg()<<var.type();
//    }

//    dataSet.save( "C:/Users/wangzhiyan/Desktop/b.mrp" );

//    dataSet.dbgShow();

    return 0;
}
