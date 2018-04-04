#include "slide1.h"
//#include "../../arith/h2_split/h2_split.h"
int robotSlide1::buildTrace( QList<D1Point> &curve
                )
{
    //! clean
    delete_all( mJointsGroup );

    //! new group
    tpvGroup *pGroup;
    for ( int i = 0; i < 1; i++ )
    {
        pGroup = new tpvGroup();
        if ( NULL == pGroup )
        { return -1; }

        mJointsGroup.append( pGroup );

        //! for each data
        for ( int j = 0; j < curve.size(); j++ )
        {
            pGroup->addItem( curve.at(j).t,
                             curve.at(j).p,
                             0 );
        }
    }

    return 0;
}
