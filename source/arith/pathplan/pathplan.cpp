#include "pathplan.h"

#define SPACE_RESOLUTION    5

namespace ns_pathplan {

static double pointDistance( double x1, double y1, double z1,
                         double x2, double y2, double z2 )
{
    double dist2;
    dist2 = pow(x1-x2,2) + pow(y1-y2,2) + pow(z1-z2,2 );
    return sqrt(dist2);
}

static double pointDistance( double *pXYZ1,
                             double *pXYZ2 )
{
    return pointDistance( pXYZ1[0], pXYZ1[1], pXYZ1[2],
                          pXYZ2[0], pXYZ2[1], pXYZ2[2] );
}

//! trim by space error
//! if the | a - b| < error
//! forbid the a.interp
static void trimPoints( double *pIn, int length )
{
    double dist;

    endPoint *pEp;
    pEp=(endPoint*)pIn;

    for ( int i = 0; i < length - 1; i++ )
    {
        dist = pointDistance( &pEp[i].x,
                              &pEp[i+1].x );
        if ( dist < SPACE_RESOLUTION )      //! disable the interp
        {
            pEp[i].flagInterp = 0;
        }
    }
}

#include "functions.cpp"

}
