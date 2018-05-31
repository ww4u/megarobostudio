#include "h2.h"
#include "../../arith/h2_split/h2_split.h"

int robotH2::verifyTrace( QList<H2KeyPoint> &curve )
{
    //! check size
    if ( curve.size() < 2 )
    { return -1; }

    //! check time
    float tPre;
    tPre = curve.at( 0 ).t;
    if ( tPre < 0 )
    { return -2; }

    for ( int i = 1; i <curve.size(); i++ )
    {
        if ( curve.at(i).t <= tPre )
        { return -2; }

        tPre = curve.at(i).t;
    }

    return 0;
}

int robotH2::buildTrace( QList<H2KeyPoint> &curve,
                         QList< tpvGroup *> &jointsGroup,
                         QList< int > &sectionList )
{

    xxxGroup< h2_split::endPoint > endPoints;

    if ( 0 != endPoints.alloc( curve.size() ) )
    { return -1; }

    //! move data
    int dir;
    for ( int i = 0; i < curve.size(); i++ )
    {
        endPoints.data()[i].t = curve.at(i).t;
//        endPoints.data()[i].x = curve.at(i).x;      //! convert x , y
//        endPoints.data()[i].y = curve.at(i).y;

        dir = ( mAngleDir.at(0) ? 1 : -1 );
        endPoints.data()[i].x = dir * curve.at(i).x + mAxesDirs.at(0) * ( mArmLengths.at(3) - mArmLengths.at(4) ) /2;    //! 409/2
        dir = ( mAngleDir.at(1) ? 1 : -1 );
        endPoints.data()[i].y = dir * curve.at(i).y + mAxesDirs.at(1) * ( mArmLengths.at(2) - mArmLengths.at(5) ) /2;    //! 802/2

        endPoints.data()[i].v = curve.at(i).v;

        logDbg()<<endPoints.data()[i].t<<endPoints.data()[i].x<<endPoints.data()[i].y;
    }

    //! split
    QList< tpvList*> splitDataSet;
    int ret;
    QList<double> zeroXy;
    zeroXy.append( mZeroX );
    zeroXy.append( mZeroY );
    ret = h2_split::h2Split( mArmLengths,
                             zeroXy,
                             endPoints.data(),
                             curve.size(),
                             splitDataSet );

    if ( ret != 0 )
    {
        logDbg()<<mZeroX<<mZeroY;
        sysLog( "desolve fail" );
        h2_split::h2Gc( splitDataSet );
        return ret;
    }

    //! to tpv group
    delete_all( jointsGroup );
    ret = 0;

    //! for each column
    tpvGroup *pGroup;
    for ( int i = 0; i < splitDataSet.size(); i++ )
    {
        pGroup = new tpvGroup();
        if ( NULL == pGroup )
        {logDbg();
            ret = -1;
            break;
        }

        jointsGroup.append( pGroup );

        //! for each item
        for ( int j = 0; j < splitDataSet.at(i)->size(); j++ )
        {
            ret = pGroup->addItem(
                                splitDataSet.at(i)->at(j)->mT,
                                splitDataSet.at(i)->at(j)->mP,
                                splitDataSet.at(i)->at(j)->mV
                        );
            if ( ret != 0 )
            { break; }

            logDbg()<<splitDataSet.at(i)->at(j)->mT
                    <<splitDataSet.at(i)->at(j)->mP
                    <<splitDataSet.at(i)->at(j)->mV;
        }
    }

    //! clean
    h2_split::h2Gc( splitDataSet );

    if ( ret != 0 )
    {
        delete_all( jointsGroup );
        return ret;
    }

    //! add section
    sectionList<<0<<splitDataSet.size();

    return 0;
}
