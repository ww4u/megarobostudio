#include "motionedit.h"
#include "ui_motionedit.h"

#include "../../arith/pathplan/pathplan.h"
#include "../../arith/kinematic/kinematic.h"

void motionEdit::on_btnGraph_clicked()
{
    if ( mJointsPlan.size() > 0 )
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

void motionEdit::updatePlot()
{
    //! now for the joints
    int skipUnit;
    skipUnit = sizeof(jointsTrace)/sizeof(double);
    for ( int i = 0; i < 4; i++ )
    {
        m_pPlot->setCurve( QString("t-p%1").arg( i + 1 ), &mJointsPlan.data()->t, skipUnit,
                         &mJointsPlan.data()->p[i], skipUnit,
                         mJointsPlan.size() );

        m_pPlot->setCurve( QString("t-v%1").arg( i + 1 ), &mJointsPlan.data()->t, skipUnit,
                         &mJointsPlan.data()->v[i], skipUnit,
                         mJointsPlan.size() );
    }


    skipUnit = sizeof(tracePoint)/sizeof(double);

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
