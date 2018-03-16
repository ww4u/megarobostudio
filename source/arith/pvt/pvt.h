#ifndef _PVT_INTERP_H_
#define _PVT_INTERP_H_

#include <QtCore>

enum enumInterpMode
{
    e_interp_cubic = 0,     //! \see PlanModeEnum
    e_interp_linear = 1,

    e_interp_trapzoid = 3,
};

struct interpContext
{
    qreal mPStep;                   //! p step
    int   mSteps;                   //! step count

    qreal mtNow;                    //! tNow
    qreal mMicroSteps;              //! micro steps count

                                    //! output
    QList< QPointF > *mp_tVs;       //! [t,V]
    QList< QPointF > *mp_tPs;       //! [t,P]

    qreal mvNow;
};

struct interpConfig
{
    int mSteps;
    int mVernierStep;
    int mEncoderLines;
    int mSlowRatio;
};

//! 0 -- no error
int pvtInterp(  enumInterpMode eInterp,
                const QPointF &p1,     //! t,p1
                const QPointF &p2,     //! t,p2

                interpConfig &config,

                interpContext &context,

                                //! out
                QList< QPointF > &tp,
                QList< QPointF > &tv
               );


int pvtInterp(  enumInterpMode eInterp,
                qreal t1, qreal p1,
                qreal t2, qreal p2,
                interpConfig &config,
                                //! out
                QList< QPointF > &tp,
                QList< QPointF > &tv
               );

int pvtInterp(  enumInterpMode eInterp,
                const QList<QPointF> &ends,
                interpConfig &config,

                                //! out
                QList< QPointF > &tp,
                QList< QPointF > &tv);

#endif
