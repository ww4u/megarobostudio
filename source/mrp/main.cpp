#include <QCoreApplication>

#include "mdataset.h"
#include "../../include/mcstd.h"

int main( int argc, char **argv )
{
    QCoreApplication app( argc, argv );

    MDataSet dataSet;


    dataSet.load("C:/Users/wangzhiyan/Desktop/a.mrp");

    MDataSection *pSec;
    pSec = dataSet.section( 0 );
    QVariant var;
    for ( int i = 0; i < dataSet.columns(); i++ )
    {
        var = pSec->cell( 0, i );
        logDbg()<<var.type();
    }

    dataSet.save( "C:/Users/wangzhiyan/Desktop/b.mrp" );

//    dataSet.dbgShow();

    return 0;
}
