#ifndef _BASE_TYPE_H_
#define _BASE_TYPE_H_

#define tpvType float
#define time_eq( a, b )     ( fabs( (a)-(b) ) <= 1e-6f )
class tpvRow{
public:
        union
        {
            struct
            {
                tpvType mT;
                tpvType mP;
                tpvType mV;
            };
            tpvType datas[3];
        };
        bool mbGc;              //! gc on used
public:
        tpvRow( )
        {
            mbGc = false;
            mT = 0;
            mP = 0;
            mV = 0;
        }

        void setGc( bool b )
        { mbGc = b; }

        bool gc()
        { return mbGc; }
};

class mcRow
{
public:
    tpvType mT,mX,mY,mZ;
};

#endif
