
#include <QApplication>
#include <QDebug>

//#include "../../../arith/pvt/GelgoogProject/PVT/Sources/pvtAlgorithm.h"

//extern "C" u8 pvtPointCalc(u8 chanNum, PvtCalcStruct *pPvtCalcData);

////! output data
//static void outputData( uint8_t chanNum,
//                 OutpDataTypeEnum datatype,
//                 uint32_t outpData,
//                 BufferOperateEnum bufferOperate )
//{
//    qDebug()<<chanNum<<(int)datatype<<outpData<<(int)bufferOperate;
//}

#include "../../../com/comassist.h"

int __main(int argc, char *argv[])
{
    double v[8]={0,1,2,3,4,5,6,7 };
    double o[5];

    dataCompress( v, 8, 1, o, 2 );

//    for ( int i = 0; i < 3; i++)
//    { printf( "%g\n", o[i]); }

    return 0;
}

#include "../../../arith/pvt/pvt.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int ret;

    QList< QPointF > tps, tvs;
//    ret = pvtInterp( 0, 0, 5, 90,
//                     64.0*200*10/360,
//                     tps,
//                     tvs );

    QList< QPointF > ends;
    ends.append( QPointF(0,0) );
    ends.append( QPointF(5,90) );
    ends.append( QPointF(10,45) );
    ends.append( QPointF(15,180) );
    ends.append( QPointF(20,-360) );

    interpConfig iConfig;
    iConfig.mEncoderLines = 2000;
    iConfig.mSteps = 200;
    iConfig.mVernierStep = 64;
    iConfig.mSlowRatio = 10;

//    ret = pvtInterp( e_interp_cubic, ends, iConfig, tps, tvs );
    ret = pvtInterp( e_interp_trapzoid, ends, iConfig, tps, tvs );
//    ret = pvtInterp( e_interp_linear, ends, iConfig, tps, tvs );

    qDebug()<<ret<<tps.size()<<tvs.size();
    if ( ret != 0 )
    { return 0; }

    QFile file("E:/trash/a.csv");
    file.open( QIODevice::WriteOnly );
    QTextStream textStream( &file );

    for ( int i = 0; i < tvs.size(); i++ )
    {
        textStream<<tps[i].x()<<","<<tps[i].y()<<","<<tvs[i].y()<<"\n";
//        textStream<<tvs[i].x()<<","<<tvs[i].y()<<"\n";
    }

    file.close();

    return 0;
}
