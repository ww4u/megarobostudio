#include "pvtedit.h"
#include "ui_pvtedit.h"

#include "tpvplot.h"

#include "../../device/mrq/deviceMRQ.h"
#include "../../../include/mcstd.h"
#include "../../../sys/sysapi.h"

#include "../../../arith/pvt/pvt.h"

pvtEdit::pvtEdit(QWidget *parent) :
    tableEdit(parent),
    ui(new Ui::pvtEdit)
{
    ui->setupUi(this);

    mFilePattern<<pvt_desc<<pvt_ext;

    m_pWndKnob = NULL;
    m_pProgress = NULL;

    mTStep = 1;
    mPStep = 1;

    mCurT = 0;
    mCurP = 0;
    mCurV = 1;

    mTpvGroup = NULL;

    m_pPlot = NULL;

    setupUi();

    buildConnection();
}

pvtEdit::~pvtEdit()
{
    gcLine();

    delete ui;
}

void pvtEdit::setModelObj( mcModelObj *pObj )
{
    Q_ASSERT( NULL != pObj );
    Q_ASSERT( pObj->getObj() );

    modelView::setModelObj( pObj );

    mTpvGroup = (tpvGroup*)pObj->getObj();

    ui->tableView->setModel( mTpvGroup );

    connect( mTpvGroup, SIGNAL(signal_data_changed()),
             this, SLOT(slot_data_changed()) );

    slot_data_changed();
}

int pvtEdit::save( QString &outFileName )
{
    outFileName = mTpvGroup->getPath() + "/" + mTpvGroup->getName();

    return mTpvGroup->save( outFileName );
}

int pvtEdit::saveAs( QString &outFileName )
{
    return mTpvGroup->save( outFileName );
}

void pvtEdit::onNetEvent(const QString &name,
                        int axes,
                        RoboMsg &msg)
{
    logDbg()<<msg.getMsg();

    //! event id, frame id, byte array
    if ( msg.getMsg() == e_interrupt_occuring )
    {
        //! status
        if ( msg.at(0).toInt() == event_status  )
        {
            QByteArray ary;
            ary = msg.at(2).toByteArray();
            onMotionStatus( axes, (MRQ_MOTION_STATE)ary.at(3) );
        }
    }
    else if ( msg.getMsg() == e_download_started )
    {
        progress()->setRange( 0, 100 );
        progress()->setWindowTitle( QString("%1-%2").arg(name).arg(axes) );
        progress()->show();

        ui->btnDown->setEnabled( false );
    }
    else if ( msg.getMsg() == e_download_processing )
    {
        int now, total;

        now = msg.at(2).toInt();
        total = msg.at(3).toInt();

        progress()->setRange( 0, total );
        progress()->setValue( now );

        logDbg()<<now<<total;
    }
    else if ( msg.getMsg() == e_download_completed )
    {
        progress()->hide();
        delete progress();
        m_pProgress = NULL;

        ui->btnDown->setEnabled( true );
    }
    else if ( msg.getMsg() == e_download_terminated )
    {
        sysLog( msg.at(0).toString(),
                QString("%1").arg( msg.at(1).toInt()),
                tr("Terminated") );

        ui->btnDown->setEnabled( true );
    }
    else
    {}
}

void pvtEdit::onMotionStatus( int axes, MRQ_MOTION_STATE stat )
{
    if ( stat == MRQ_MOTION_STATE_IDLE )
    {
        ui->btnDown->setEnabled( true );
        ui->btnStart->setEnabled( false );
        ui->btnStop->setEnabled( false );
    }
    else if ( stat == MRQ_MOTION_STATE_CALCEND )
    {
        ui->btnDown->setEnabled( true );
        ui->btnStart->setEnabled( false );
        ui->btnStop->setEnabled( false );
    }
    else if ( stat == MRQ_MOTION_STATE_STANDBY )
    {
        ui->btnDown->setEnabled( true );
        ui->btnStart->setEnabled( true );
        ui->btnStop->setEnabled( false );
    }
    else if ( stat == MRQ_MOTION_STATE_RUNNING )
    {
        ui->btnDown->setEnabled( false );
        ui->btnStart->setEnabled( false );
        ui->btnStop->setEnabled( true );
    }
    else
    {
        ui->btnDown->setEnabled( true );
        ui->btnStart->setEnabled( false );
        ui->btnStop->setEnabled( false );
    }
}

void pvtEdit::setupUi()
{
    //! varialbes
    ui->cmbPlanMode->addItem( tr("Linear"), ( (int)1 ) );
    ui->cmbPlanMode->addItem( tr("Cubic"), ( (int)0 ) );
    ui->cmbPlanMode->addItem( tr("Trapezoid"), ( (int)3 ) );
}

void pvtEdit::buildConnection()
{
    connect( this, SIGNAL(sigLineChanged()),
             this, SLOT(slot_line_changed()) );
}

bool pvtEdit::checkChan( const QString &name,
                         int axesId )
{
    Q_ASSERT( NULL != m_pmcModel );

    if ( NULL != m_pmcModel->m_pInstMgr->findDevice( name, axesId ) )
    { return true; }
    else
    { return false; }
}

bool pvtEdit::checkChan()
{
    QString str;
    int id;
    str = m_pmcModel->getConnection().getDeviceName();
    id = m_pmcModel->getConnection().getDeviceCH();

    bool b;
    b = checkChan( str, id );

    //! success
    if ( b )
    {
        setAgent( str, id );
        setLink( true );
    }
    //! fail
    else
    {
        sysError( QString("Invalid Device:%1 %2")
                  .arg(str)
                  .arg(id) );

        setLink(false);
    }

    return b;
}

void pvtEdit::context_remove()
{
    on_btnDel_clicked();
}

void pvtEdit::context_clear()
{
    on_btnClr_clicked();
}

void pvtEdit::context_add_before()
{
    int curRow;

    //! current
    curRow = ui->tableView->currentIndex().row();

    mCurT += mTStep;
    mCurP += mPStep;

    mTpvGroup->insertRow( curRow );
    (*mTpvGroup)[ curRow ]->set( mCurT, mCurP, mCurV );

    ui->tableView->setCurrentIndex( mTpvGroup->index( curRow, 0) );
}
void pvtEdit::context_add_below()
{
    on_btnAdd_clicked();
}

//! download
int pvtEdit::postDownload( appMsg msg, void *pPara )
{
    QString str;
    int axesId;
    str = m_pmcModel->getConnection().getDeviceName();
    axesId = m_pmcModel->getConnection().getDeviceCH();

    MegaDevice::deviceMRQ *pMrq = m_pmcModel->m_pInstMgr->findDevice( str,
                                                                      axesId );
    Q_ASSERT( NULL != pMrq );

    //! set loop count
    int ret;
    ret = pMrq->setMOTIONPLAN_CYCLENUM( axesId, ui->spinLoop->value() );
    if ( ret != 0 )
    { return ret; }

    QList<tpvRow*> tpvRows;
    mTpvGroup->getRows( tpvRows );

    //! download
    ret = pMrq->pvtWrite( axesId, tpvRows );
    if ( ret != 0 )
    { return ret; }

    return 0;
}
void pvtEdit::beginDownload( void *pPara )
{
}
void pvtEdit::endDownload( int ret, void *pPara )
{
}

void pvtEdit::progDownload( int now, int from, int to, void *pPara )
{
}

//! start
int pvtEdit::postStart( appMsg msg, void *pPara )
{
    QString str;
    int axesId;
    str = m_pmcModel->getConnection().getDeviceName();
    axesId = m_pmcModel->getConnection().getDeviceCH();

    MegaDevice::deviceMRQ *pMrq = m_pmcModel->m_pInstMgr->findDevice( str,
                                                                      axesId );
    Q_ASSERT( NULL != pMrq );

    MRQ_MOTION_STATE stat;

    int ret = pMrq->getMOTION_STATE( axesId, &stat );
    if ( ret != 0 )
    { return ret; }
    if ( stat != MRQ_MOTION_STATE_STANDBY )
    { return ERR_CAN_NOT_RUN; }

    ret = pMrq->run( axesId );

    return ret;
}
void pvtEdit::beginStart( void *pPara )
{}
void pvtEdit::endStart( int ret, void *pPara )
{}

//! stop
int pvtEdit::postStop( appMsg msg, void *pPara )
{
    QString str;
    int axesId;
    str = m_pmcModel->getConnection().getDeviceName();
    axesId = m_pmcModel->getConnection().getDeviceCH();

    MegaDevice::deviceMRQ *pMrq = m_pmcModel->m_pInstMgr->findDevice( str,
                                                                      axesId );
    Q_ASSERT( NULL != pMrq );

    int ret;
    ret = pMrq->stop( axesId );

    return ret;
}
void pvtEdit::beginStop( void *pPara )
{}
void pvtEdit::endStop( int ret, void *pPara )
{}

QProgressDialog *pvtEdit::progress()
{
    if ( NULL != m_pProgress )
    { }
    else
    {
        m_pProgress = new QProgressDialog(this);
        m_pProgress->setAutoClose( false );
        m_pProgress->setAutoReset( false );

        connect( m_pProgress,
                 SIGNAL(canceled()),
                 this,
                 SLOT(slot_download_cancel()) );
    }

    Q_ASSERT( NULL != m_pProgress );

    return m_pProgress;
}

int pvtEdit::buildLine()
{
    int ret;

    gcLine();           //! 0

    ret = checkLine();  //! 1
    if ( ret != 0 )
    { return ret; }
                        //! 2
    ret = compileLine();
    return ret;
}

void pvtEdit::gcLine()
{
    mTPs.clear();
    mTVs.clear();
}

int pvtEdit::checkLine()
{
    //! get data
    Q_ASSERT( NULL != mTpvGroup );
    QList< tpvItem *> *pRows;
    pRows = mTpvGroup->getRows();
    if ( NULL == pRows )
    {
        sysError( "Invalid dot" );
        return ERR_INVALID_DATA;
    }

    if ( pRows->size() < 2 )
    {
        sysError( "Invalid dot" );
        return ERR_INVALID_DATA;
    }

    tpvItem *pItem;
    float fT = 0;
    for ( int i = 0; i < pRows->size(); i++ )
    {
        pItem = pRows->at( i );
        Q_ASSERT( NULL != pItem );

        //! check input
        if ( pItem->getT() < fT )
        {
            sysError( "Invalid time at line ", QString::number(i) );
            return ERR_INVALID_DATA;
        }
        else
        {
            fT = pItem->getT();
        }
    }

    return 0;
}
int pvtEdit::compileLine()
{
    int dotLine;

    QList< tpvItem *> *pRows;
    pRows = mTpvGroup->getRows();

    dotLine = pRows->size();

    //! copy data
    tpvItem *pItem;
    QList< QPointF > ends;
    for ( int i = 0; i < dotLine; i++ )
    {
        pItem = pRows->at( i );
        Q_ASSERT( NULL != pItem );
        if ( pItem->getValid() )
        { ends.append( QPointF( pItem->getT(), pItem->getP() ) ); }
    }

    //! compile
    int ret;
    interpConfig config;
    config.mEncoderLines = mPvtPref.mEncoderLines;
    config.mSlowRatio = mPvtPref.mSlowRatio;
    config.mSteps = mPvtPref.mSteps;
    config.mVernierStep = mPvtPref.mVernierStep;

    ret = pvtInterp( (enumInterpMode)ui->cmbPlanMode->value(),
                     ends,
                     config,

                     mTPs,
                     mTVs );
    if ( ret != 0 )
    {
        sysError("compile fail");
        return ret;
    }
    else
    { }

    return 0;
}

//! udpate the data
void pvtEdit::updatePlot()
{
    m_pPlot->setDumpDir( m_pmcModel->mSysPref.mDumpPath );

    m_pPlot->setCurve( tr("t-p"), mTPs );
    m_pPlot->setCurve( tr("t-v"), mTVs );

}

void pvtEdit::on_btnBuild_clicked()
{
    int ret = buildLine();
    if ( ret == 0 )
    {
        sysLog( "Line build completed", QString::number( mTPs.size()), QString::number( mTVs.size() ) );
        emit sigLineChanged();
    }
}

void pvtEdit::on_btnDown_clicked()
{
    if ( !checkChan() )
    { return; }

    post_request_prog( msg_download_pvt, pvtEdit, Download );
}
void pvtEdit::on_btnStart_clicked()
{
    if ( !checkChan() )
    { return; }

    post_request( msg_start_pvt, pvtEdit, Start );
}
void pvtEdit::on_btnStop_clicked()
{
    if ( !checkChan() )
    { return; }

    post_request( msg_stop_pvt, pvtEdit, Stop );
}

//! add below
void pvtEdit::on_btnAdd_clicked()
{
    int curRow;

    //! current
    curRow = ui->tableView->currentIndex().row();

    mCurT += mTStep;
    mCurP += mPStep;

    mTpvGroup->insertRow( curRow + 1 );
    (*mTpvGroup)[ curRow + 1 ]->set( mCurT, mCurP, mCurV );

    ui->tableView->setCurrentIndex( mTpvGroup->index( curRow + 1, 0) );
}

void pvtEdit::on_btnDel_clicked()
{
    mTpvGroup->removeRow( ui->tableView->currentIndex().row() );
}

void pvtEdit::on_btnClr_clicked()
{
    QMessageBox msgBox;
    msgBox.setText( tr("Sure to delete all?") );
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel );
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    {
        mTpvGroup->removeRows( 0, mTpvGroup->rowCount(QModelIndex()), QModelIndex() );
    }
}

void pvtEdit::on_btnGraph_clicked()
{
    //! create the plot
    if ( NULL != m_pPlot )
    {}
    else
    {
        m_pPlot = new tpvPlot(this);
    }

    if ( NULL == m_pPlot )
    { return; }

    updatePlot();

    m_pPlot->show();
}

void pvtEdit::on_btnKnob_clicked()
{
    if ( !checkChan() )
    { return; }

    if ( m_pWndKnob == NULL )
    {
        m_pWndKnob = new axesKnob(this);
        Q_ASSERT( NULL != m_pWndKnob );
    }

    //! set model && axesid
    QString str;
    int id;
    str = m_pmcModel->getConnection().getDeviceName();
    id = m_pmcModel->getConnection().getDeviceCH();

    MegaDevice::deviceMRQ *pMrq = m_pmcModel->m_pInstMgr->findDevice( str,
                                                                      id );
    m_pWndKnob->setDevice( pMrq, id );
    m_pWndKnob->setConnection( QString("CH%1@%2").arg(id + 1 ).arg( str ) );

    m_pWndKnob->show();
}

void pvtEdit::slot_download_cancel()
{
    //! set model && axesid
    QString str;
    int id;
    str = m_pmcModel->getConnection().getDeviceName();
    id = m_pmcModel->getConnection().getDeviceCH();

    MegaDevice::deviceMRQ *pMrq = m_pmcModel->m_pInstMgr->findDevice( str, id );
    pMrq->terminate( mAgentAxes );
}

void pvtEdit::on_spinLoop_valueChanged(int arg1)
{
    if ( !checkChan() )
    { return; }

    QString str;
    int axesId;
    str = m_pmcModel->getConnection().getDeviceName();
    axesId = m_pmcModel->getConnection().getDeviceCH();

    MegaDevice::deviceMRQ *pMrq = m_pmcModel->m_pInstMgr->findDevice( str,
                                                                      axesId );
    Q_ASSERT( NULL != pMrq );

    pMrq->setMOTIONPLAN_CYCLENUM( axesId, arg1 );
}

void pvtEdit::on_btnPref_clicked()
{
    PvtPref dlgPref(this);

    dlgPref.setModel( &mPvtPref );
    dlgPref.setModal( true );
    dlgPref.exec();
}

void pvtEdit::slot_data_changed()
{
    Q_ASSERT( NULL != mTpvGroup );

    if ( mTpvGroup->rowCount(QModelIndex()) < 2 )       //! not enough points
    {
        ui->btnBuild->setEnabled( false );
    }
    else
    {
        ui->btnBuild->setEnabled( true );
    }

                                                        //! ui en
    bool bHasItem = mTpvGroup->rowCount(QModelIndex()) > 0;
    ui->btnDel->setEnabled( bHasItem );
    ui->btnClr->setEnabled( bHasItem );

    m_pContextActionClear->setEnabled( bHasItem );
    m_pContextActionDelete->setEnabled( bHasItem );
}

void pvtEdit::slot_line_changed()
{
    if ( mTPs.size() > 0 )
    {
        ui->btnDown->setEnabled( true );
        ui->btnGraph->setEnabled( true );

        //! update plot
        if ( m_pPlot != NULL && m_pPlot->isVisible() )
        { updatePlot(); }
    }
    else
    {
        ui->btnDown->setEnabled( false);
        ui->btnGraph->setEnabled( false );
    }
}


