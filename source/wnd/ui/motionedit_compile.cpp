#include "motionedit.h"
#include "ui_motionedit.h"

int motionEdit::doCompile()
{
    preCompileTrace();

    int ret;
    ret = compileTrace();

    postCompileTrace( ret );

    return ret;
}

int motionEdit::compileTrace()
{
    int ret;

    //! compile
    emit sigSaveRequest( this );

    ret = checkRobot();
    if ( ret != 0 )
    { return ret; }

    //! plan attr
    currentRobot()->setPlanAttr( PlanAttr( plan_linear, ui->spinStep->value() ) );
    currentRobot()->setCoord( (eRoboCoord)ui->coordOption->option() );

    ret = currentRobot()->build( m_pMotionGroup,
                                 mTracePlan,
                                 mJointsPlan,
                                 mJointsTpvGroup,
                                 mSectionList
                                 );
    if ( ret != 0 )
    { return ret; }

    return ret;
}

int motionEdit::preCompileTrace()
{
    delete_all( mJointsTpvGroup );

    mTracePlan.clear();
    mJointsPlan.clear();
    mSectionList.clear();

    return 0;
}

int motionEdit::postCompileTrace( int ret )
{
    //! fail clean the resource
    if ( ret != 0 )
    {
        delete_all( mJointsTpvGroup );

        mTracePlan.clear();
        mJointsPlan.clear();
        sysError( tr("Build Fail" ) );
    }
    else
    {
        sysLog( tr("Build success"),
                QString::number( mTracePlan.size()),
                QString::number( mJointsPlan.size()),
                QString::number( mJointsTpvGroup.size()) );
    }

    emit sig_joints_trace_changed();

    return 0;
}

int motionEdit::checkRobot()
{
    VRobot *pRobo;

    //! check robo
    pRobo = currentRobot();
    if ( NULL == pRobo )
    {
        sysError( tr("invalid robot name"),  ui->widget->roboName() );
        return ERR_INVALID_ROBOT_NAME;
    }

    //! model match
    Q_ASSERT( NULL != m_pMotionGroup );
    if ( pRobo->getClass().compare( m_pMotionGroup->className(), Qt::CaseInsensitive ) != 0 )
    {
        sysError( tr("robo do not match"), pRobo->getClass(), m_pMotionGroup->className() );
        return ERR_INVALID_ROBOT_NAME;
    }

    return 0;
}
