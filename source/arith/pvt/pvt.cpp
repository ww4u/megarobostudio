
#include "pvt.h"
#include "../../../include/mydebug.h"

#include "./GelgoogProject/PVT/Sources/pvtAlgorithm.h"

//! for c api
//extern "C" u8 pvtPointCalc(u8 chanNum, PvtCalcStruct *pPvtCalcData);
extern "C"
u8 pvtSegmentCalc(PvtCalcStruct *pPvtCalcData, u8 chanNum);

//! 20M Hz
#define norm_freq               (20000000)
#define width_to_speed( wid )   ((float)norm_freq/wid)

static void outputData( uint8_t /*chanNum*/,
                 OutpDataTypeEnum /*datatype*/,
                 uint32_t outpData,
                 BufferOperateEnum /*bufferOperate*/,
                 void */*pPvtCalcData*/,
                 void *pContext )
{
//    return;

    //! check
    Q_ASSERT( pContext != NULL );

    qreal vNow, tDur;
    interpContext *pInterpContext;

    //! decode
    pInterpContext = (interpContext*)pContext;
    Q_ASSERT( pInterpContext->mp_tPVs != NULL );

    //! v now
    //! pInterpContext->mMicroSteps =  THAT.posnToStep
    vNow = width_to_speed( outpData ) / pInterpContext->mMicroSteps ;

    vNow = vNow * ( pInterpContext->mPStep >=0 ? 1 : -1 );

    //! acc t
    tDur = (qreal)outpData/norm_freq * ((PvtCalcStruct*)(pInterpContext->m_pCalcContext))->lineSteps;
    Q_ASSERT( NULL != pInterpContext->m_pCalcContext );
    PvtCalcStruct *pCalcContext;
    pCalcContext = (PvtCalcStruct*)pInterpContext->m_pCalcContext;
    pInterpContext->mtNow += tDur;

//    localContext.mSteps = qAbs( p2.y() - p1.y() ) * THAT.posnToStep;
//    localContext.mPStep = ( p2.y() - p1.y() ) / localContext.mSteps;
    //! acc p
    pInterpContext->mpNow += pInterpContext->mPStep;

    //! append
    pInterpContext->mp_tPVs->append( QVector3D( pInterpContext->mtNow,
                                                pInterpContext->mpNow,
                                                vNow ) );

    //! reg
    pInterpContext->mvNow = vNow;
}

#define THAT pvtDatas
void pvtInterpInit( PvtCalcStruct &pvtDatas,

                    enumInterpMode eInterp,
                    interpConfig &config,
                    interpContext &localContext,
                                             //! out
                    QList< QVector3D > &tpv
                    )
{
    //! that
    memset( &THAT, 0, sizeof(PvtCalcStruct) );

    //! config
    THAT.bQueryReady = false;
    THAT.bReportCalcEnd = false;

    THAT.pvtPlanMode    =  (PlanModeEnum)eInterp;
    THAT.pvtExecMode    =  EXECMODE_NCYCLE;
    THAT.motionMode     =  (MotionModeEnum)config.mMotionMode;

    THAT.lastStepDir = OUTPDATA_FORWARD;
    THAT.lastPoint = 0;

    THAT.fpgaPwmClock = norm_freq;
    THAT.fpgaClockOffset = 0;

    THAT.startPosn 				= 0;
    THAT.startSpeed 			= 0;
    THAT.startTime				= 0;
    THAT.endPosn				= 0;
    THAT.endSpeed				= 0;
    THAT.motionTime				= 0;
    THAT.motionStep				= 0;
    THAT.invsePosition			= 0;
    THAT.waitPosition			= 0;
    THAT.waitStepError			= 0;
    THAT.targetStep				= 1;
    THAT.lastStepSpeed			= 0;
    THAT.maxOffset				= 0.02;
    THAT.minOffset				= -0.02;
    THAT.posnToStep				= config.mVernierStep * config.mSteps * config.mSlowRatio / 360.0;
                                //! 35.5555572;
                                //! 64*200*10/360 = 355.55...
    if ( THAT.motionMode == MTNMODE_PVT )
    { THAT.lineSteps = 1; }
    else
    { THAT.lineSteps		    = config.mVernierStep * config.mSteps / config.mEncoderLines; }
                                //! 1
                                //! 64*200/2000;
    THAT.lineStepsInv			= 1/THAT.lineSteps;
    THAT.errorTime				= 0;

    THAT.startPoint.position   = 0;
    THAT.startPoint.speed      = 0;
    THAT.startPoint.time       = 0;
    THAT.startPoint.accScale   = 300;
    THAT.startPoint.decScale   = 300;

    THAT.endPoint.position   = 0;
    THAT.endPoint.speed      = 0;
    THAT.endPoint.time       = 0;
    THAT.endPoint.accScale   = 300;
    THAT.endPoint.decScale   = 300;

    THAT.timeCount = 0;

    //! intf
    THAT.outpBufferFill = outputData;
    THAT.pContext = &localContext;

    //! context
    localContext.mp_tPVs = &tpv;
    localContext.m_pCalcContext = &THAT;
}

void pvtInterpDeInit( PvtCalcStruct &pvtDatas )
{

}

//! 0 -- no error
int pvtInterpProc(
                PvtCalcStruct &pvtDatas,

                const QVector3D &p1,     //! t,p1,v1
                const QVector3D &p2,     //! t,p2,v2

                interpConfig &config,
                interpContext &localContext,

                QList< QVector3D > &tpv
               )
{
    int ret;

    //! only for p
    THAT.startPoint.position   = p1.y();
    THAT.startPoint.speed      = p1.z();
    THAT.startPoint.time       = p1.x();
    THAT.startPoint.accScale   = config.mAcc;
    THAT.startPoint.decScale   = config.mDec;

    THAT.endPoint.position   = p2.y();
    THAT.endPoint.speed      = p2.z();
    THAT.endPoint.time       = p2.x();
    THAT.endPoint.accScale   = config.mAcc;
    THAT.endPoint.decScale   = config.mDec;

    //! now for context
    Q_ASSERT( THAT.posnToStep != 0  );
    localContext.mSteps = qAbs( p2.y() - p1.y() ) * THAT.posnToStep;

    Q_ASSERT( localContext.mSteps >= 0  );                       //! t * v = pStep
    if ( localContext.mSteps > 0 )
    { localContext.mPStep = ( p2.y() - p1.y() ) / localContext.mSteps; }
    //! == 0
    else
    {
        tpv.append( QVector3D( p2.x(), p2.y(), p2.z() ) );

        localContext.mtNow = p2.x();
        localContext.mpNow = p2.y();
        localContext.mvNow = p2.z();

        return 0;
    }

    localContext.mMicroSteps = THAT.posnToStep;                   //! step count

    //! calc
    ret = pvtSegmentCalc( &THAT, 0 );
    logDbg()<<ret;
    return ret;
}

int pvtInterp(  enumInterpMode eInterp,
                const QList<QVector3D> &ends,
                interpConfig &config,
                                //! out
                QList< QVector3D > &tpv )
{
    int ret;

    if ( ends.size() < 2 )
    { return -1; }

    //! context
    interpContext localContext;

    localContext.mtNow = ends[0].x();
    localContext.mpNow = ends[0].y();
    localContext.mvNow = ends[0].z();

    //! control
    PvtCalcStruct calcCon;
    pvtInterpInit( calcCon,
                   eInterp,
                   config,
                   localContext,
                   tpv );

    //! a->b
    for ( int i = 0; i < ends.size() - 1; i++ )
    {
        ret = pvtInterpProc(
                         calcCon,

                         ends[i],
                         ends[i+1],

                         config,
                         localContext,

                         tpv );

        if ( ret != 0 )
        { break; }
    }

    pvtInterpDeInit( calcCon );

    return ret;
}

//! 0 -- no error
int pvtInterp( enumInterpMode eInterp,
               qreal t1, qreal p1,
               qreal t2, qreal p2,
               interpConfig &config,
                                //! out
               QList< QVector3D > &tpv
               )
{
    QList<QVector3D> ends;

    ends.append( QVector3D(t1,p1,0) );
    ends.append( QVector3D(t2,p2,0) );

    return pvtInterp( eInterp, ends, config, tpv );
}


//! for slope
int  GetCalcResult(double* pvt, int len, int mode, double* percent,int* index);

int pvtSlope( double p1, double v1, double t1,
              double p2, double v2, double t2,
              int planMode,
              double slopes[2],
              int *pIndex )
{
    double pvt[6] = {
                        p1, v1, t1,
                        p2, v2, t2 };


    return 0;
//    return GetCalcResult( pvt, 2, planMode, slopes, pIndex );
}
