
#include "pvt.h"


#include "./GelgoogProject/PVT/Sources/pvtAlgorithm.h"

//! for c api
extern "C" u8 pvtPointCalc(u8 chanNum, PvtCalcStruct *pPvtCalcData);


//! 20M Hz
#define norm_freq               (20000000.0)
#define width_to_speed( wid )   (norm_freq/wid)

static void outputData( uint8_t /*chanNum*/,
                 OutpDataTypeEnum /*datatype*/,
                 uint32_t outpData,
                 BufferOperateEnum /*bufferOperate*/,
                 void *pContext )
{
//    qDebug()<<chanNum<<(int)datatype<<outpData<<(int)bufferOperate;

    //! check
    Q_ASSERT( pContext != NULL );

    qreal vNow, tDur;
    interpContext *pInterpContext;

    //! decode
    pInterpContext = (interpContext*)pContext;
    Q_ASSERT( pInterpContext->mp_tVs != NULL );

    //! v now
    vNow = width_to_speed( outpData ) / pInterpContext->mMicroSteps ;
    tDur = pInterpContext->mPStep * pInterpContext->mMicroSteps * outpData / norm_freq;

    //! acc v
    Q_ASSERT( NULL != pInterpContext->m_pCalcContext );
    PvtCalcStruct *pCalcContext;
    pCalcContext = (PvtCalcStruct*)pInterpContext->m_pCalcContext;
    if ( pCalcContext->lastStepDir == DIR_FORWARD )
    { pInterpContext->mtNow += tDur; }
    else
    { pInterpContext->mtNow -= tDur; }

    //! acc p
    pInterpContext->mpNow += pInterpContext->mPStep;

    //! append
    pInterpContext->mp_tVs->append( QPointF( pInterpContext->mtNow, vNow ) );
    pInterpContext->mp_tPs->append( QPointF( pInterpContext->mtNow, pInterpContext->mpNow ) );

    //! reg
    pInterpContext->mvNow = vNow;
}

//! 0 -- no error
int pvtInterp(  enumInterpMode eInterp,
                const QPointF &p1,     //! t,p1
                const QPointF &p2,     //! t,p2

                interpConfig &config,

                interpContext &localContext,
                                //! out
                QList< QPointF > &tp,
                QList< QPointF > &tv
               )
{

    //! local para
    PvtCalcStruct pvtDatas;

    PvtCalcStruct *pPvtCalcData;

    pPvtCalcData = &pvtDatas;
    memset( pPvtCalcData, 0, sizeof(PvtCalcStruct) );

    //
    //
    //
    pPvtCalcData->pvtPlanMode    =  (PlanModeEnum)eInterp;
    pPvtCalcData->pvtExecMode    =  EXECMODE_NCYCLE;
    pPvtCalcData->motionMode     =  MTNMODE_PVT;

    //! current dir
    pPvtCalcData->lastStepDir    =  p2.y() >= p1.y() ? DIR_FORWARD : DIR_REVERSE;

    //! \note end > start
    pPvtCalcData->startPosn      =  p1.y();
    pPvtCalcData->endPosn        =  pPvtCalcData->startPosn + qAbs( p2.y() - p1.y() );

    pPvtCalcData->lastPoint    =  2;
    pPvtCalcData->accScale    =  300;        //千分之
    pPvtCalcData->decScale    =  300;        //千分之
    pPvtCalcData->fpgaPwmClock  =  0;
    //
    pPvtCalcData->startSpeed      =  0.0;
    pPvtCalcData->startTime        =  0;
    pPvtCalcData->endSpeed          =  0.0;
    pPvtCalcData->motionTime      =  p2.x() - p1.x();

    pPvtCalcData->invsePosition  =  0;
    pPvtCalcData->waitPosition    =  0;
    pPvtCalcData->waitStepError  =  0;
    pPvtCalcData->targetStep        =  1;
    pPvtCalcData->lastStepSpeed  =  0;
    pPvtCalcData->lastStepTime    =  0;
    pPvtCalcData->targetLine  =  1;
    //
    //
    //
    pPvtCalcData->lineErrorP  =  0.0003f;
    pPvtCalcData->lineErrorN  =  -0.0003f;
    pPvtCalcData->errorTime    =  0;
    //
    //
    pPvtCalcData->posnConvertInfo.posnToStep     //! 64*200*10/360 = 355.55...
                                              = config.mVernierStep * config.mSteps * config.mSlowRatio / 360.0;

    pPvtCalcData->posnConvertInfo.posnToLine  =  //2000/360;      //! 单倍乘编码器，一圈2000线，一圈360度
                                                config.mEncoderLines / 360.0;

    pPvtCalcData->posnConvertInfo.lineSteps   =  //64*200/2000;
                                                config.mVernierStep * config.mSteps / config.mEncoderLines;
    pPvtCalcData->posnConvertInfo.lineMult    =  0;
    pPvtCalcData->outpBufferFill = outputData;
    pPvtCalcData->pContext = &localContext;                     //! context

    //! now for context
    Q_ASSERT( pPvtCalcData->posnConvertInfo.posnToStep != 0  );
    localContext.mSteps = qAbs( p2.y() - p1.y() ) * pPvtCalcData->posnConvertInfo.posnToStep;

    Q_ASSERT( localContext.mSteps > 0  );                       //! t * v = pStep
    localContext.mPStep = ( p2.y() - p1.y() ) / localContext.mSteps;
    localContext.mMicroSteps = pPvtCalcData->posnConvertInfo.posnToStep;                           //! step count

    localContext.mp_tPs = &tp;
    localContext.mp_tVs = &tv;
    localContext.m_pCalcContext = pPvtCalcData;

    //! p1 -> p2
//    tp.append( p1 );
//    tp.append( p2 );

    //! calc
    int ret;
    ret = pvtPointCalc( 0, pPvtCalcData );

//    qDebug()<<localContext.mtNow<<localContext.mvNow;

    return ret;
}

//! 0 -- no error
int pvtInterp( enumInterpMode eInterp,
               qreal t1, qreal p1,
               qreal t2, qreal p2,
               interpConfig &config,
                                //! out
               QList< QPointF > &tp,
               QList< QPointF > &tv
               )
{
    interpContext localContext;

    localContext.mtNow = t1;
    localContext.mpNow = p1;

    return pvtInterp( eInterp,
                      QPointF( t1, p1 ),
                      QPointF( t2, p2),
                      config,
                      localContext,

                      tp,
                      tv
                      );
}


int pvtInterp(  enumInterpMode eInterp,
                const QList<QPointF> &ends,
                interpConfig &config,
                                //! out
                QList< QPointF > &tp,
                QList< QPointF > &tv)
{
    int ret;

    if ( ends.size() < 2 )
    { return -1; }

    interpContext localContext;

    localContext.mtNow = ends[0].x();
    localContext.mpNow = ends[0].y();

    //! a->b
    for ( int i = 0; i < ends.size() - 1; i++ )
    {
        ret = pvtInterp( eInterp,
                         ends[i], ends[i+1], config,
                         localContext,

                         tp, tv );

        if ( ret != 0 )
        { return ret; }
    }

    return 0;
}
