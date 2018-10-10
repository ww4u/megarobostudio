#include "tpedit.h"
#include "ui_tpedit.h"

#include "../../widget/megamessagebox.h"

TpEdit::TpEdit(QWidget *parent) :
    tableEdit(parent),
    ui(new Ui::TpEdit)
{
    ui->setupUi(this);

    mFilePattern<<pt_desc<<pt_ext;

    tableEdit::setupUi();

    m_pTpGroup = NULL;
    m_pPlot = NULL;
    connect( this, SIGNAL(sigLineChanged()),
             this, SLOT(slot_line_changed()) );
}

TpEdit::~TpEdit()
{
    delete ui;
}

void TpEdit::setModelObj( mcModelObj *pObj )
{
    Q_ASSERT( NULL != pObj );
    Q_ASSERT( pObj->getObj() );

    modelView::setModelObj( pObj );

    m_pTpGroup = (TpGroup*)pObj->getObj();

    ui->tableView->setModel( m_pTpGroup );

    connect( m_pTpGroup, SIGNAL(signal_data_changed()),
             this, SLOT(slot_data_changed()) );

    slot_data_changed();
}

int TpEdit::save( QString &outFileName )
{
    outFileName = m_pTpGroup->getPath() + QDir::separator() + m_pTpGroup->getName();
    outFileName = QDir::toNativeSeparators( outFileName );
    return m_pTpGroup->save( outFileName );
}

int TpEdit::saveAs( QString &outFileName )
{
    return m_pTpGroup->save( outFileName );
}

void TpEdit::context_remove()
{
    on_btnDel_clicked();
}

void TpEdit::context_clear()
{
    on_btnClr_clicked();
}

void TpEdit::onNetEvent(const QString &name,
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
            onMotionStatus( axes, (MRQ_MOTION_STATE_2)ary.at(4) );
        }
    }
    else
    {}
}

void TpEdit::onMotionStatus( int axes, MRQ_MOTION_STATE_2 stat )
{
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

bool TpEdit::checkChan( const QString &name,
                         int axesId )
{
    Q_ASSERT( NULL != m_pmcModel );

    VRobot *pRobo;
    pRobo = m_pmcModel->m_pInstMgr->findRobot( name, axesId );
    if ( NULL != pRobo && robot_is_mrv( pRobo->mId ) )
    {
        return true;
    }
    else
    { return false; }
}

bool TpEdit::checkChan()
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

MegaDevice::deviceMRV *TpEdit::currentDevice( int &ax )
{
    QString str;
    int axesId;

    str = m_pmcModel->getConnection().getDeviceName();
    axesId = m_pmcModel->getConnection().getDeviceCH();

    VRobot *pRobo = m_pmcModel->m_pInstMgr->findRobot( str, axesId );
    MegaDevice::deviceMRV *pDev = (MegaDevice::deviceMRV*)pRobo;

    ax = axesId;
    return pDev;
}

void TpEdit::gcLine()
{
    mTPs.clear();
}

bool TpEdit::checkLine()
{
    if ( NULL == m_pTpGroup )
    { return false; }

    QList< TpItem *> *pRows = m_pTpGroup->getRows();
    if ( NULL == pRows )
    {
        sysError( tr("Invalid dot") );
        return false;
    }

    if ( pRows->size() < 2 )
    {
        sysError( tr("Invalid dot") );
        return false;
    }

    TpItem *pItem;
    float fT = 0;
    for ( int i = 0; i < pRows->size(); i++ )
    {
        pItem = pRows->at( i );
        Q_ASSERT( NULL != pItem );

        //! check input
        if ( pItem->getT() < fT )
        {
            sysError( tr("Invalid time at line "), QString::number(i) );
            return false;
        }
        else
        { fT = pItem->getT(); }
    }

    return true;
}

int TpEdit::buildLine()
{
    //! 0
    gcLine();

    //! 1. check the line
    if ( !checkLine() )
    { return -1; }

    //! 2. append
    Q_ASSERT(NULL!=m_pTpGroup);
    QList< TpItem *> *pRows = m_pTpGroup->getRows();
    TpItem *pItem;
    QPointF pt;
    for ( int i = 0; i < pRows->size(); i++ )
    {
        pItem = pRows->at( i );
        Q_ASSERT(NULL!=pItem);
        pt.setX( pItem->getT() );
        pt.setY( pItem->getP() );
        mTPs.append( pt );
    }

    return 0;
}

//! udpate the data
void TpEdit::updatePlot()
{
    m_pPlot->setDumpDir( m_pmcModel->mSysPref.mDumpPath );

    m_pPlot->setCurve( tr("t-p"), mTPs );
}

//! download
int TpEdit::postDownload( appMsg msg, void *pPara )
{
    //! device
    int ax;
    MegaDevice::deviceMRV *pDev = currentDevice( ax );
    Q_ASSERT( NULL != pDev );

    //! set loop count
    int ret;
    ret = pDev->setPVT_CYCLES( ax, ui->spinLoop->value() );
    if ( ret != 0 )
    { return ret; }

    QList<TpRow*> tpRows;
    m_pTpGroup->getRows( tpRows );

    //! download
    ret = pDev->tpWrite( tpRows, ax );
    if ( ret != 0 )
    { return ret; }

    return 0;
}
void TpEdit::beginDownload( void *pPara )
{
}
void TpEdit::endDownload( int ret, void *pPara )
{
}

void TpEdit::context_add_before()
{
    int curRow;

    //! current
    curRow = ui->tableView->currentIndex().row();

    Q_ASSERT( NULL != m_pTpGroup );
    m_pTpGroup->insertRow( curRow );

    ui->tableView->setCurrentIndex( m_pTpGroup->index( curRow, 0) );
}
void TpEdit::context_add_below()
{
    on_btnAdd_clicked();
}

//! slots
void TpEdit::slot_data_changed()
{
    Q_ASSERT( NULL != m_pTpGroup );

    if ( m_pTpGroup->rowCount(QModelIndex()) < 2 )       //! not enough points
    { ui->btnBuild->setEnabled( false ); }
    else
    { ui->btnBuild->setEnabled( true ); }
                                                        //! ui en
    bool bHasItem = m_pTpGroup->rowCount(QModelIndex()) > 0;
    ui->btnDel->setEnabled( bHasItem );
    ui->btnClr->setEnabled( bHasItem );

    m_pContextActionClear->setEnabled( bHasItem );
    m_pContextActionDelete->setEnabled( bHasItem );
}

void TpEdit::slot_download_cancel()
{
}

void TpEdit::slot_line_changed()
{
    if ( mTPs.size() > 0  )
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

void TpEdit::on_btnBuild_clicked()
{
    int ret = buildLine();
    if ( ret == 0 )
    {
        sysLog( tr("Line build completed"), QString::number( mTPs.size()) );
        emit sigLineChanged();
    }
}

void TpEdit::on_btnDown_clicked()
{
    if ( !checkChan() )
    { return; }

    post_request( msg_download_pt, TpEdit, Download );
}
void TpEdit::on_btnStart_clicked()
{
    if ( !checkChan() )
    { return; }

    int ax;
    MegaDevice::deviceMRV *pDev = currentDevice( ax );
    if ( NULL == pDev )
    { return; }

    pDev->switchRun( ax );

    //! disable start
    ui->btnStart->setEnabled( false );
    ui->btnStop->setEnabled( true );
}
void TpEdit::on_btnStop_clicked()
{
    if ( !checkChan() )
    { return; }

    int ax;
    MegaDevice::deviceMRV *pDev = currentDevice( ax );
    if ( NULL == pDev )
    { return; }

    pDev->switchStop( ax );

    //! disable start
    ui->btnStart->setEnabled( true );
    ui->btnStop->setEnabled( false );
}

//! add below
void TpEdit::on_btnAdd_clicked()
{
    int curRow;

    //! current
    curRow = ui->tableView->currentIndex().row();

    Q_ASSERT( NULL != m_pTpGroup );
    m_pTpGroup->insertRow( curRow + 1 );

    ui->tableView->setCurrentIndex( m_pTpGroup->index( curRow + 1, 0) );
}

void TpEdit::on_btnDel_clicked()
{
    Q_ASSERT( NULL != m_pTpGroup );
    m_pTpGroup->removeRow( ui->tableView->currentIndex().row() );
}

void TpEdit::on_btnClr_clicked()
{
    MegaDeleteAffirmMessageBox msgBox;
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    {
        Q_ASSERT( NULL != m_pTpGroup );
        m_pTpGroup->removeRows( 0,
                                m_pTpGroup->rowCount(QModelIndex()), QModelIndex() );
    }
}

void TpEdit::on_btnGraph_clicked()
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

void TpEdit::on_spinLoop_valueChanged(int arg1)
{
    if ( !checkChan() )
    { return; }

    int ax;
    MegaDevice::deviceMRV *pDev = currentDevice( ax );
    if ( NULL == pDev )
    { return; }

    pDev->setPVT_CYCLES( ax, ui->spinLoop->value() );
}

void TpEdit::on_btnPref_clicked()
{
}

void TpEdit::on_comboBox_currentIndexChanged(int index)
{
    Q_ASSERT( m_pTpGroup != NULL );

    m_pTpGroup->setViewMode( (TpGroup::eViewMode)index ) ;

    int ax;
    MegaDevice::deviceMRV *pDev = currentDevice( ax );
    if ( NULL == pDev )
    { return; }

    //! set device
    pDev->setVALVECTRL_DEVICE( ax, (MRV_VALVECTRL_DEVICE)index );
}
