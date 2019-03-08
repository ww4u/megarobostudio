#include "motionedit.h"
#include "ui_motionedit.h"

//#include "../../arith/pathplan/pathplan.h"
//#include "../../arith/kinematic/kinematic.h"

void motionEdit::on_btnGraph_clicked()
{
    if ( mJointsTpvGroup.size() > 0 )
    {}
    else
    { return; }

    if ( m_pPlot != NULL )
    {
    }
    else
    {
        m_pPlot = new tpvPlot( this );
        if ( NULL == m_pPlot )
        { return; }
    }

    m_pPlot->setDumpDir( m_pmcModel->mSysPref.mDumpPath );

    updatePlot();

    m_pPlot->show();
}

void motionEdit::on_btnReverse_clicked()
{
    Q_ASSERT( NULL != m_pMotionGroup );
    m_pMotionGroup->reverse();
}

void motionEdit::on_btnAutoTime_clicked()
{
    Q_ASSERT( NULL != m_pMotionGroup );

    //! auto time
    m_pMotionGroup->autoTime( m_pmcModel->mSysPref.mMaxSpeed * ui->spinSpeed->value()/100,
                              m_pmcModel->mSysPref.mMaxTerminalSpeed * ui->spinSpeed->value()/100,

                              mAligns.at( ui->comboBox->currentIndex() ),
                              mDivs.at( ui->comboBox->currentIndex() )
                              );
}

void motionEdit::updatePlot()
{
    for ( int i = 0; i < mJointsTpvGroup.size(); i++ )
    {
        tpvGroupPlot( mJointsTpvGroup.at(i), i+1 );
    }

    //! trace plan
    if ( mTracePlan.size() > 0 )
    {
        int skipUnit = sizeof(tracePoint)/sizeof(double);

        m_pPlot->setCurve( "t-x", &mTracePlan.data()->t, skipUnit,
                                &mTracePlan.data()->x, skipUnit,
                                mTracePlan.size() );
        m_pPlot->setCurve( "t-y", &mTracePlan.data()->t, skipUnit,
                                &mTracePlan.data()->y, skipUnit,
                                mTracePlan.size() );
        m_pPlot->setCurve( "t-z", &mTracePlan.data()->t, skipUnit,
                                &mTracePlan.data()->z, skipUnit,
                                mTracePlan.size() );

        //! xyz
        double *xyz[]={ &mTracePlan.data()->x,
                        &mTracePlan.data()->y,
                        &mTracePlan.data()->z };
        int skipYs[ ] = { skipUnit,skipUnit,skipUnit } ;

        QStringList names;
        names<<"x"<<"y"<<"z";
        m_pPlot->setCurves( "t-xyz",
                            names,
                           &mTracePlan.data()->t, skipUnit,
                           xyz, skipYs,
                           sizeof_array(skipYs),
                           mTracePlan.size() );
    }
}

void motionEdit::tpvGroupPlot( tpvGroup *pGroup, int id )
{
    Q_ASSERT( NULL != pGroup );

    if ( pGroup->mItems.size() > 0 )
    {}
    else
    { return; }

    //! trans data
    xxxGroup< double > ts, ps, vs;
    if ( 0!= ts.alloc( pGroup->mItems.size() ) )
    { return; }
    if ( 0!= ps.alloc( pGroup->mItems.size() ) )
    { return; }
    if ( 0!= vs.alloc( pGroup->mItems.size() ) )
    { return; }

    for ( int i = 0; i < pGroup->mItems.size(); i++ )
    {
        ts.data()[i] = pGroup->mItems.at(i)->getT();
        ps.data()[i] = pGroup->mItems.at(i)->getP();
        vs.data()[i] = pGroup->mItems.at(i)->getV();
    }

    m_pPlot->setCurve( QString("t-p%1").arg( id ),
                       ts.data(),1,
                       ps.data(),1,
                       pGroup->mItems.size() );

    m_pPlot->setCurve( QString("t-v%1").arg( id ),
                       ts.data(),1,
                       vs.data(),1,
                       pGroup->mItems.size() );

    ts.clear();
    ps.clear();
    vs.clear();
}

bool motionEdit::checkRobotLive( VRobot **ppRobot )
{
    Q_ASSERT( NULL != ppRobot );

    VRobot *pRobo;
logDbg();
    //! check robo
    pRobo = currentRobot();
    if ( NULL == pRobo )
    {
        sysError( tr("Invalid robot name"),  m_pmcModel->mConn.getRoboName() );
        return false;
    }
logDbg();
    if ( pRobo->checkLink() )
    {}
    else
    {
        sysError( tr("Invalid robot link"), pRobo->name() );
        return false;
    }

    *ppRobot = pRobo;

    return true;
}
