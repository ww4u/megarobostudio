#include "pvtedit.h"
#include "ui_pvtedit.h"

#include "tpvplot.h"

#include "../../device/mrq/deviceMRQ.h"
#include "../../../include/mcstd.h"
#include "../../../sys/sysapi.h"

#include "../../../arith/pvt/pvt.h"
#include "../../widget/megamessagebox.h"



pvtEdit::pvtEdit(QWidget *parent) :
    tableEdit(parent),
    ui(new Ui::pvtEdit)
{
    ui->setupUi(this);

    mFilePattern<<pvt_desc<<pvt_ext;

    m_pProgress = NULL;

    mTStep = 1;
    mPStep = 1;

    mCurT = 0;
    mCurP = 0;
    mCurV = 0;

    mTpvGroup = NULL;

    m_pPlot = NULL;

    setupUi();

    buildConnection();

    //! delegate
    m_pSpinDelegate = new dSpinDelegate( this );
    Q_ASSERT( NULL != m_pSpinDelegate );
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
    ui->tableView->setItemDelegateForColumn( 1, m_pSpinDelegate );
    ui->tableView->setItemDelegateForColumn( 2, m_pSpinDelegate );
    ui->tableView->setItemDelegateForColumn( 3, m_pSpinDelegate );

    connect( mTpvGroup, SIGNAL(signal_data_changed()),
             this, SLOT(slot_data_changed()) );

    slot_data_changed();
}

int pvtEdit::save( QString &outFileName )
{
    outFileName = mTpvGroup->getPath() + QDir::separator() + mTpvGroup->getName();
    outFileName = QDir::toNativeSeparators( outFileName );
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
//    logDbg()<<msg.getMsg();

    //! event id, frame id, byte array
    if ( msg.getMsg() == e_interrupt_occuring )
    {
        //! status
        if ( msg.at(0).toInt() == event_status  )
        {
            QByteArray ary;
            ary = msg.at(3).toByteArray();
            onMotionStatus( axes, (MRQ_MOTION_STATE_2)ary.at(4) );
        }
    }
    else
    {}
}

void pvtEdit::onMotionStatus( int axes, MRQ_MOTION_STATE_2 stat )
{logDbg()<<stat;/*sysLog(__FUNCTION__, QString::number( (int)stat ) );*/
    if ( stat == MRQ_MOTION_STATE_2_IDLE )
    {
        ui->btnDown->setEnabled( true );
        ui->btnStart->setEnabled( false );
        ui->btnStop->setEnabled( false );
    }
    else if ( stat == MRQ_MOTION_STATE_2_CALCEND )
    {
        ui->btnDown->setEnabled( true );
        ui->btnStart->setEnabled( true );
        ui->btnStop->setEnabled( false );
    }
    else if ( stat == MRQ_MOTION_STATE_2_STANDBY )
    {
        ui->btnDown->setEnabled( false );
        ui->btnStart->setEnabled( false );
        ui->btnStop->setEnabled( false );
    }
    else if ( stat == MRQ_MOTION_STATE_2_RUNNING )
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
    ui->cmbPlanMode->addItem( tr("Trapezoid"), ( (int)3 ) );
    ui->cmbPlanMode->addItem( tr("Linear"), ( (int)1 ) );
    ui->cmbPlanMode->addItem( tr("Cubic"), ( (int)0 ) );
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
        sysError( QString( tr("Invalid Device:%1 %2") )
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
    int axesId, page;
    str = m_pmcModel->getConnection().getDeviceName();
    axesId = m_pmcModel->getConnection().getDeviceCH();
    page = m_pmcModel->getConnection().devicePage();

    MegaDevice::deviceMRQ *pMrq = m_pmcModel->m_pInstMgr->findDevice( str,
                                                                      axesId );
    Q_ASSERT( NULL != pMrq );

    //! set loop count
    int ret;
    ret = pMrq->setMOTIONPLAN_CYCLENUM( axesId,
                                        (MRQ_MOTION_SWITCH_1)page,
                                        ui->spinLoop->value() );
    if ( ret != 0 )
    { return ret; }

    QList<tpvRow*> tpvRows;
    mTpvGroup->getRows( tpvRows );

    //! download
    ret = pMrq->pvtWrite( tpvRegion(axesId, page ), tpvRows );
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

//    MRQ_MOTION_STATE_2 stat;

//    int ret = pMrq->getMOTION_STATE( axesId, MRQ_MOTION_SWITCH_1_MAIN, &stat );
//    if ( ret != 0 )
//    { return ret; }
//    if ( stat != MRQ_MOTION_STATE_2_CALCEND )
//    { return ERR_CAN_NOT_RUN; }
    int ret;
    ret = pMrq->run( tpvRegion(axesId,0) );

    return ret;
}
void pvtEdit::beginStart( void *pPara )
{}
void pvtEdit::endStart( int ret, void *pPara )
{
    if ( ret != 0 )
    { sysError( tr("start fail") ); }
    else
    { }
}

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
    ret = pMrq->stop( tpvRegion(axesId,0) );

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
        sysError( tr("Invalid dot") );
        return ERR_INVALID_DATA;
    }

    if ( pRows->size() < 2 )
    {
        sysError( tr("Invalid dot") );
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
            sysError( tr("Invalid time at line "), QString::number(i) );
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
        sysError( tr("build fail") );
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

    //! dump t-pv
    Q_ASSERT( mTPs.size() == mTVs.size() );
    Q_ASSERT( mTPs.size() > 0 );

    int size = mTPs.size();
    double *pT, *pP, *pV;
    pT = NULL;
    pP = NULL;
    pV = NULL;

    do
    {
        pT = new double[ size ];
        pP = new double[ size ];
        pV = new double[ size ];

        if ( NULL == pT || NULL == pP || NULL == pV )
        { break; }

        //! export
        for ( int i = 0; i < size; i++ )
        {
            pT[i] = mTPs.at(i).x();
            pP[i] = mTPs.at(i).y();
            pV[i] = mTVs.at(i).y();
        }

        //! plot
        double *pv[]={ pP, pV };
        int skipYs[ ] = { 1,1 } ;

        QStringList names;
        names<<"p"<<"v";
        m_pPlot->setCurves( "t-pv",
                            names,
                           pT, 1,
                           pv, skipYs,
                           sizeof_array(skipYs),
                           size );
    }while( 0 );

    gc_array3( pT, pP, pV );
}

void pvtEdit::on_btnBuild_clicked()
{
    int ret = buildLine();
    if ( ret == 0 )
    {
        sysLog( tr("Line build completed"), QString::number( mTPs.size()), QString::number( mTVs.size() ) );
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

    //! diable start
    ui->btnStart->setEnabled( false );

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

    mTpvGroup->insertRow( curRow + 1 );
    (*mTpvGroup)[ curRow + 1 ]->set( mCurT, mCurP, mCurV );

    mCurT += mTStep;
    mCurP += mPStep;

    ui->tableView->setCurrentIndex( mTpvGroup->index( curRow + 1, 0) );
}

void pvtEdit::on_btnDel_clicked()
{
    mTpvGroup->removeRow( ui->tableView->currentIndex().row() );

//    QItemSelectionModel *pModel = ui->tableView->selectionModel();

//    QModelIndexList selectList = pModel->selectedIndexes();
//    foreach( QModelIndex index, selectList )
//    {
//        mTpvGroup->removeRow( index.row() );
//    }
}

void pvtEdit::on_btnClr_clicked()
{
    MegaDeleteAffirmMessageBox msgBox;
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

void pvtEdit::slot_download_cancel()
{
    //! set model && axesid
    QString str;
    int id;
    str = m_pmcModel->getConnection().getDeviceName();
    id = m_pmcModel->getConnection().getDeviceCH();

    MegaDevice::deviceMRQ *pMrq = m_pmcModel->m_pInstMgr->findDevice( str, id );
    pMrq->terminate( tpvRegion(mAgentAxes,0) );
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

    pMrq->setMOTIONPLAN_CYCLENUM( axesId, MRQ_MOTION_SWITCH_1_MAIN, arg1 );
}

void pvtEdit::on_comboBox_currentIndexChanged(int index)
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

    pMrq->setMOTIONPLAN_CYCLENUM( axesId,
                                  MRQ_MOTION_SWITCH_1_MAIN,
                                  (MRQ_MOTIONPLAN_ENDSTATE_1)index );
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




