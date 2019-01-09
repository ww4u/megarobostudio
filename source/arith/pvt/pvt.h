#ifndef _PVT_INTERP_H_
#define _PVT_INTERP_H_

#include <QtCore>
#include <QVector3D>

enum enumInterpMode
{
    e_interp_cubic = 0,     //! \see PlanModeEnum
    e_interp_trapzoid = 1,
    e_interp_sine = 2,
};

struct interpContext
{
    qreal mPStep;                   //! p step
    int   mSteps;                   //! step count

    qreal mtNow;                    //! tNow
    qreal mpNow;
    qreal mvNow;

    qreal mMicroSteps;              //! micro steps count

                                    //! output
    QList< QVector3D > *mp_tPVs;    //! [t,p,V]
    void *m_pCalcContext;
};

struct interpConfig
{
    int mMotionMode;

    int mSteps;
    int mVernierStep;
    int mEncoderLines;
    int mSlowRatio;

    int mAcc, mDec;
};

int pvtInterp(  enumInterpMode eInterp,
                qreal t1, qreal p1,
                qreal t2, qreal p2,
                interpConfig &config,
                                //! out
                QList< QVector3D > &tpv
               );

int pvtInterp(  enumInterpMode eInterp,
                const QList<QVector3D> &ends,
                interpConfig &config,

                                //! out
                QList< QVector3D > &tpv );

int pvtSlope( double p1, double v1, double t1,
              double p2, double v2, double t2,
              int planMode,
              double slopes[2],
              int *pIndex );

#endif
