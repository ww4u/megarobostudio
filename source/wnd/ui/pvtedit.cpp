#include "pvtedit.h"
#include "ui_pvtedit.h"

#include "tpvplot.h"
#include "../../com/comassist.h"

#include "../../device/mrq/deviceMRQ.h"
#include "../../../include/mcstd.h"
#include "../../../sys/sysapi.h"

#include "../../../arith/pvt/pvt.h"
#include "../../widget/megamessagebox.h"



#define _get_dst_meta( ret )  \
QString _deviceName; \
int _axesId, _axesPage; \
ui->widget->connectionAttr( _deviceName, _axesId, _axesPage );\
MegaDevice::deviceMRQ *_pMrq = m_pmcModel->m_pInstMgr->findDevice( _deviceName,\
                                                                  _axesId );\
if ( NULL == _pMrq )\
{\
    sysError( QObject::tr("Invalid device ") + _deviceName );\

#define get_dst_meta( )         _get_dst_meta() \
                                return; \
                                }
#define get_dst_meta_ret( ret ) _get_dst_meta() \
                                return ret; }

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

    //! timer
    mDlySaveTimer.setSingleShot( true );
    mDlySaveTimer.setInterval( 5000 );

    setupUi();

    buildConnection();

    //! delegate
    m_pSpinDelegate = new dSpinDelegate( this );
    Q_ASSERT( NULL != m_pSpinDelegate );

    m_ptDelegate = new dSpinDelegate(this);
    Q_ASSERT( NULL != m_ptDelegate );
    m_ptDelegate->setMin( 0 );

    //! check box
    m_pboolDelegate =new checkDelegate(shape_check,this);
    Q_ASSERT( NULL != m_pboolDelegate );

    //! linked
    setLink( true );
}

pvtEdit::~pvtEdit()
{
    gcLine();

    delete ui;
}

void pvtEdit::changeEvent(QEvent * event)
{
    tableEdit::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi( this );

        retranslateContextMenu();
    }
}

void pvtEdit::setModelObj( mcModelObj *pObj )
{
    Q_ASSERT( NULL != pObj );
    Q_ASSERT( pObj->getObj() );

    modelView::setModelObj( pObj );

    mTpvGroup = (tpvGroup*)pObj->getObj();

    ui->tableView->setModel( mTpvGroup );

    //! time type
    ui->comboBox->setCurrentIndex( mTpvGroup->getTimeType() );
    logDbg()<<mTpvGroup->getTimeType();

    ui->tableView->setItemDelegateForColumn( 0, m_pboolDelegate );
    ui->tableView->setItemDelegateForColumn( 1, m_ptDelegate );
    ui->tableView->setItemDelegateForColumn( 2, m_pSpinDelegate );
    ui->tableView->setItemDelegateForColumn( 3, m_pSpinDelegate );

    connect( mTpvGroup, SIGNAL(signal_data_changed()),
             this, SLOT(slot_data_changed()) );

    slot_data_changed();

    adapteAxes();
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

//! setting
void pvtEdit::settingChanged( enumSetting setting, const QVariant &v )
{logDbg()<<setting;
    if ( setting == modelView::setting_inst_mgr )
    { onMcModelUpdated(); }
    else if ( setting == modelView::setting_mrq_motion_unit )
    { slot_destination_changed(); }
    else
    {}
}

void pvtEdit::onMcModelUpdated()
{
    Q_ASSERT( NULL != m_pmcModel );
    ui->widget->setDeviceNames( m_pmcModel->mConn.mDeviceMap );

    //! updated
    //! dst changed
    connect( ui->widget, SIGNAL(signal_changed()),
             this, SLOT(slot_destination_changed()) );

    //! changed
    if ( m_pmcModel->mConn.mDeviceMap.size() > 0 )
    { slot_destination_changed(); }
}

QString pvtEdit::activeName()
{ return ui->widget->getDeviceName(); }
int pvtEdit::activeAxes()
{ return ui->widget->getDeviceCH(); }

void pvtEdit::onNetEvent(const QString &name,
                         int axes,
                         RoboMsg &msg)
{
    //! event id, frame id, byte array
    if ( msg.getMsg() == e_interrupt_occuring )
    {
        //! status
        if ( msg.at(0).toInt() == event_status  )
        {
            QByteArray ary;
            ary = msg.at(3).toByteArray();
            onMotionStatus( name, axes, (MRQ_MOTION_STATE_2)ary.at(4) );
        }
    }
    else
    {}
}

void pvtEdit::onMotionStatus( const QString &name, int axes, MRQ_MOTION_STATE_2 stat )
{logDbg()<<axes<<stat<<name;

    if ( str_is( name, ui->widget->getDeviceName() ) )
    {}
    else
    { return; }

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
    ui->cmbPlanMode->addItem( tr("Cubic"), ( (int)0 ) );
    ui->cmbPlanMode->addItem( tr("Trapezoid"), ( (int)1 ) );
    ui->cmbPlanMode->addItem( tr("S curve"), ( (int)2 ) );
}

void pvtEdit::buildConnection()
{
    connect( this, SIGNAL(sigLineChanged()),
             this, SLOT(slot_line_changed()) );

    connect( &mDlySaveTimer, SIGNAL(timeout()),
             this, SLOT(slot_timeout()) );
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
//    QString str;
//    int id;
//    str = m_pmcModel->getConnection().getDeviceName();
//    id = m_pmcModel->getConnection().getDeviceCH();

    get_dst_meta_ret( false );

    bool b;
    b = checkChan( _deviceName, _axesId );

    //! success
    if ( b )
    {
        setAgent( _deviceName, _axesId );
        setLink( true );
    }
    //! fail
    else
    {
        sysError( QString( tr("Invalid Device:%1 %2") )
                  .arg(_deviceName)
                  .arg(_axesId) );

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
    get_dst_meta_ret( -1 )

//    str = m_pmcModel->getConnection().getDeviceName();
//    axesId = m_pmcModel->getConnection().getDeviceCH();
//    page = m_pmcModel->getConnection().devicePage();

//    str = ui->widget->getDeviceName();
//    axesId = ui->widget->getDeviceCH();
//    page = ui->widget->devicePage();

//    MegaDevice::deviceMRQ *pMrq = m_pmcModel->m_pInstMgr->findDevice( _deviceName,
//                                                                      _axesId );
////    Q_ASSERT( NULL != pMrq );
//    if ( NULL == pMrq )
//    {
//        sysError( QObject::tr("Invalid device ") + str );
//        return -1;
//    }

    //! set loop count
    int ret;
    ret = _pMrq->setMOTIONPLAN_CYCLENUM( _axesId,
                                        (MRQ_MOTION_SWITCH_1)_axesPage,
                                        ui->spinLoop->value() );
    if ( ret != 0 )
    { return ret; }

    //! plan mode
    ret = _pMrq->setMOTIONPLAN_PLANMODE( _axesId,
                                        (MRQ_MOTION_SWITCH_1)_axesPage,
                                        (MRQ_MOTIONPLAN_PLANMODE_1)ui->cmbPlanMode->currentIndex() );
    if ( ret != 0 )
    { return ret; }

    //! motion mode
    ret = _pMrq->setMOTIONPLAN_MOTIONMODE( _axesId,
                                        (MRQ_MOTION_SWITCH_1)_axesPage,
                                        (MRQ_MOTIONPLAN_MOTIONMODE_1)ui->cmbMotionMode->currentIndex() );
    if ( ret != 0 )
    { return ret; }

    //! trim
    tpvGroup trimGroup;
    mTpvGroup->trimRows( trimGroup );

    //! abs
    tpvGroup abstGroup;
    trimGroup.abstimeRows( abstGroup );

    //! valid rows
    QList<tpvRow> tpvRows;
    abstGroup.getRows( tpvRows );

    //! scale t
    for( int i = 0; i < tpvRows.size(); i++ )
    { tpvRows[i].mT = ( tpvRows[i].mT ); }

    //! download
    ret = _pMrq->pvtWrite( tpvRegion(_axesId, _axesPage ), tpvRows );
    if ( ret != 0 )
    { return ret; }

    return 0;
}
void pvtEdit::beginDownload( void *pPara )
{
}
void pvtEdit::endDownload( int ret, void *pPara )
{
    if ( ret != 0 )
    {
        ui->btnDown->setEnabled( true );
        ui->btnStart->setEnabled( false );
        ui->btnStop->setEnabled( false );
    }
    else
    {}
}

void pvtEdit::progDownload( int now, int from, int to, void *pPara )
{
}

//! start
int pvtEdit::postStart( appMsg msg, void *pPara )
{
//    QString str;
//    int axesId;
//    str = m_pmcModel->getConnection().getDeviceName();
//    axesId = m_pmcModel->getConnection().getDeviceCH();

    get_dst_meta_ret( -1 );

    MegaDevice::deviceMRQ *pMrq = m_pmcModel->m_pInstMgr->findDevice( _deviceName,
                                                                      _axesId );
    Q_ASSERT( NULL != pMrq );
    int ret;
    ret = pMrq->run( tpvRegion(_axesId, _axesId ) );

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
//    QString str;
//    int axesId;
//    str = m_pmcModel->getConnection().getDeviceName();
//    axesId = m_pmcModel->getConnection().getDeviceCH();

    get_dst_meta_ret( -1 );

//    MegaDevice::deviceMRQ *pMrq = m_pmcModel->m_pInstMgr->findDevice( _deviceName,
//                                                                      _axesId );
//    Q_ASSERT( NULL != pMrq );

    int ret;
    //! \note for all page
    for( int page = 0; page < x_pages; page++ )
    { ret = _pMrq->stop( tpvRegion(_axesId, page ) ); }

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
        Q_ASSERT( NULL != m_pProgress );

        m_pProgress->setAutoClose( false );
        m_pProgress->setAutoReset( false );

        connect( m_pProgress,
                 SIGNAL(canceled()),
                 this,
                 SLOT(slot_download_cancel()) );
    }

    return m_pProgress;
}

int pvtEdit::buildLine()
{
    int ret;

                        //! compile
    emit sigSaveRequest( this );

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
    Q_ASSERT( NULL != mTpvGroup );

    //! trim invalid
    tpvGroup trimGp;
    mTpvGroup->trimRows( trimGp );
    if ( trimGp.getRows()->size() > 1 )
    {}
    else
    {
        sysError( tr("Invalid dot") );
        return ERR_INVALID_DATA;
    }

    //! get data
    QList< tpvItem *> *pRows;
    pRows = mTpvGroup->getRows();
    if ( NULL == pRows )
    {
        sysError( tr("Invalid dot") );
        return ERR_INVALID_DATA;
    }

    tpvItem *pItem;
    float fT = 0;
    float fAbsT;

    mTpvGroup->rstAbsT();

    for ( int i = 0; i < pRows->size(); i++ )
    {
        pItem = pRows->at( i );
        Q_ASSERT( NULL != pItem );

        //! valid
        if ( pItem->mValid )
        {}
        else
        { continue; }

        //! check input
        fAbsT = mTpvGroup->getAbsT( i );
        if ( fAbsT < fT )
        {
            sysError( tr("Invalid time at line "), QString::number(i) );
            //! to the error time
            QModelIndex index = ui->tableView->model()->index( i, 1 );
            ui->tableView->setCurrentIndex( index );
            return ERR_INVALID_DATA;
        }
        else
        { fT = fAbsT; }
    }

    return 0;
}
int pvtEdit::compileLine()
{
    //! copy data
    tpvItem *pItem;
    QList< QVector3D > ends;

    //! trim group
    tpvGroup trimGroup;
    mTpvGroup->trimRows( trimGroup );

    //! valid rows
    QList< tpvItem *> *pRows;
    pRows = trimGroup.getRows();

    int dotLine;
    dotLine = pRows->size();

    for ( int i = 0; i < dotLine; i++ )
    {
        pItem = pRows->at(i);
        Q_ASSERT( NULL != pItem );

        ends.append( QVector3D(
                                ( trimGroup.getAbsT( i ) ) * m_pmcModel->mSysPref.mTimeUnit,
                                pItem->getP(),
                                pItem->getV()
                              ) );

    }

    //! compile
    int ret;
    interpConfig config;
    config.mMotionMode = ui->cmbMotionMode->currentIndex();

    config.mEncoderLines = mPvtPref.mEncoderLines;
    config.mSlowRatio = mPvtPref.mSlowRatio;
    config.mSteps = mPvtPref.mSteps;
    config.mVernierStep = mPvtPref.mVernierStep;

    config.mAcc = mPvtPref.mAcc;
    config.mDec = mPvtPref.mDec;
logDbg();
    QList<QVector3D> tpvs;
    ret = pvtInterp( (enumInterpMode)ui->cmbPlanMode->value(),
                     ends,

                     config,

                     tpvs

                      );

    if ( ret != 0 )
    {
        sysError( tr("build fail") );

        return ret;
    }
    else
    { }
logDbg()<<tpvs.size();
    //! out
    for ( int i = 0; i < tpvs.size(); i++ )
    {
        mTPs.append( QPointF( tpvs.at(i).x(), tpvs.at(i).y() ) );
        mTVs.append( QPointF( tpvs.at(i).x(), tpvs.at(i).z() ) );

    }
logDbg();

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

//! need setting again
void pvtEdit::stateIdle()
{
    ui->btnDown->setEnabled( true );
    ui->btnStart->setEnabled( false );
    ui->btnStop->setEnabled( false );
}

void pvtEdit::adapteAxes()
{
    get_dst_meta();

//    _pMrq->requestMotionState( tpvRegion(_axesId,_axesPage) );

    //! unit
    MRQ_MOTOR_POSITIONUNIT unit;
    _pMrq->getModel()->getMOTOR_POSITIONUNIT( _axesId, &unit );
    QString strUnit;
    do
    {
        if ( unit == MRQ_MOTOR_POSITIONUNIT_ANGLE )
        { strUnit = (char_deg); }
        else if ( unit == MRQ_MOTOR_POSITIONUNIT_RADIAN )
        { strUnit = ("rad"); }
        else if ( unit == MRQ_MOTOR_POSITIONUNIT_MILLIMETER )
        { strUnit = ("mm"); }
        else
        { return; }

    }while( 0 );

    if ( ui->tableView->model() != NULL )
    {
        ui->tableView->model()->setHeaderData( 2,Qt::Horizontal, QString("(%1)").arg( strUnit ) );
        ui->tableView->model()->setHeaderData( 3,Qt::Horizontal, QString("(%1/s)").arg( strUnit ) );
    }
}

void pvtEdit::slot_timeout()
{
    emit sigSaveRequest( this );
}

void pvtEdit::on_btnBuild_clicked()
{
    int ret = buildLine();

    emit sigLineChanged();

    if ( ret == 0 )
    {
        sysLog( tr("Line build completed"), QString::number( mTPs.size()), QString::number( mTVs.size() ) );
    }
}

void pvtEdit::on_btnDown_clicked()
{
    if ( !checkChan() )
    {
        MegaMessageBox::warning( this, tr("Invalid CH"), tr("Invalid CH") );
        return;
    }

    //! build
    int ret = buildLine();

    emit sigLineChanged();

    if ( ret == 0 )
    {
        sysLog( tr("Line build completed"), QString::number( mTPs.size()), QString::number( mTVs.size() ) );
    }
    else
    {
        MegaMessageBox::warning( this, tr("Fail"), tr("Build Fail") );
        return;
    }

    //! post download
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
//    mTpvGroup->removeRow( ui->tableView->currentIndex().row() );

    QList<int> selRows;

    //! collect the rows
    QItemSelectionModel *pModel = ui->tableView->selectionModel();
    selRows = comAssist::descRows( pModel );

    foreach( int r, selRows )
    {
        mTpvGroup->removeRow( r );
    }
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
//    QString str;
//    int id;
//    str = m_pmcModel->getConnection().getDeviceName();
//    id = m_pmcModel->getConnection().getDeviceCH();

    get_dst_meta();

//    MegaDevice::deviceMRQ *pMrq = m_pmcModel->m_pInstMgr->findDevice( _deviceName, _axesId );
    _pMrq->terminate( tpvRegion(mAgentAxes, _axesPage ) );
}

void pvtEdit::on_spinLoop_valueChanged(int arg1)
{
    if ( !checkChan() )
    { return; }

//    QString str;
//    int axesId;
//    str = m_pmcModel->getConnection().getDeviceName();
//    axesId = m_pmcModel->getConnection().getDeviceCH();

    get_dst_meta();

//    MegaDevice::deviceMRQ *pMrq = m_pmcModel->m_pInstMgr->findDevice( _deviceName,
//                                                                      _axesId );
//    Q_ASSERT( NULL != pMrq );

    _pMrq->setMOTIONPLAN_CYCLENUM( _axesId,
                                  (MRQ_MOTION_SWITCH_1)_axesPage,
                                  arg1 );
}

void pvtEdit::on_cmbPlanMode_currentIndexChanged(int index)
{
    stateIdle();
}

void pvtEdit::on_cmbMotionMode_currentIndexChanged(int index)
{
    stateIdle();
}

void pvtEdit::on_btnPref_clicked()
{
    //! link to device
    if ( !checkChan() )
    { return; }

    get_dst_meta();

//    QString str;
//    int axesId;
//    str = m_pmcModel->getConnection().getDeviceName();
//    axesId = m_pmcModel->getConnection().getDeviceCH();

//    MegaDevice::deviceMRQ *pMrq = m_pmcModel->m_pInstMgr->findDevice( _deviceName,
//                                                                      _axesId );
//    if ( NULL == pMrq )
//    { return; }

    QStringList stepList;
    int stepBase;
    _pMrq->microStepAttr( stepList, stepBase );
    for ( int i = 0; i < stepBase; i++ )
    { stepList.removeAt( 0 ); }

    //! step list
    PvtPref dlgPref( stepList, this);

    dlgPref.setModel( &mPvtPref );
    dlgPref.setModal( true );
    dlgPref.exec();
}

void pvtEdit::slot_data_changed()
{
    Q_ASSERT( NULL != mTpvGroup );

    if ( mTpvGroup->rowCount(QModelIndex()) < 2 )       //! not enough points
    {
        ui->btnDown->setEnabled( false );
        ui->btnBuild->setEnabled( false );
    }
    else
    {
        ui->btnDown->setEnabled( true );
        ui->btnBuild->setEnabled( true );
    }
                                                        //! ui en
    bool bHasItem = mTpvGroup->rowCount(QModelIndex()) > 0;
    ui->btnDel->setEnabled( bHasItem );
    ui->btnClr->setEnabled( bHasItem );

    m_pContextActionClear->setEnabled( bHasItem );
    m_pContextActionDelete->setEnabled( bHasItem );

    mDlySaveTimer.start( );
}

void pvtEdit::slot_line_changed()
{
    if ( mTPs.size() > 0 )
    {
        ui->btnGraph->setEnabled( true );

        //! update plot
        if ( m_pPlot != NULL && m_pPlot->isVisible() )
        { updatePlot(); }
    }
    else
    {
        ui->btnGraph->setEnabled( false );
    }
}

void pvtEdit::slot_destination_changed()
{
    get_dst_meta();

    _pMrq->requestMotionState( tpvRegion(_axesId,_axesPage) );

    adapteAxes();
}

//! to abs or relative
void pvtEdit::on_comboBox_currentIndexChanged(int index)
{
    mTpvGroup->setTimeType( (MegaTableModel::timeType)index );
}
