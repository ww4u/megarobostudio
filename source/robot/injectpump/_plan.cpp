#include "injectpump.h"
//#include "../../arith/h2_split/h2_split.h"

int robotInject::verifyTrace( IPKeyPointList &curve )
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
        {
            sysLog( QObject::tr("Invalid time "),
                    QString::number( i ),
                    QString::number(curve.at(i).t) );
            return -2;
        }

        tPre = curve.at(i).t;
    }

    return 0;
}

int robotInject::buildTrace( IPKeyPointList &curve,
                         QList< tpvGroup *> &jointsGroup,
                         QList< int > &sectionList )
{
    tpvGroup *pGroup;
    int ret = -1;

    delete_all( jointsGroup );

    //! for v,inj
    for ( int col = 1; col < 3; col ++ )
    {
        //! a group
        pGroup = new tpvGroup();
        if ( NULL == pGroup )
        {
            delete_all( jointsGroup );
            return -1;
        }

        //! group items
        for ( int i = 0; i < curve.size(); i++ )
        {
           ret = pGroup->addItem( curve.at(i).t,
                                  curve.at(i).datas[col],
                                  curve.at(i).datas[col+2] );   //! different velocity
           if ( ret != 0 )
           { break; }
        }

        if ( ret != 0 )
        {
            delete_all( jointsGroup );
            return ret;
        }

        //! append
        jointsGroup.append( pGroup );
    }

    //! add section
    sectionList<<0<<2;

    return 0;
}
