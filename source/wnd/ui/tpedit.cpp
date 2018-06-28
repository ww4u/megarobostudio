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

void TpEdit::context_add_before()
{
    int curRow;

    //! current
    curRow = ui->tableView->currentIndex().row();

//    mCurT += mTStep;
//    mCurP += mPStep;

    Q_ASSERT( NULL != m_pTpGroup );
    m_pTpGroup->insertRow( curRow );
//    (*mTpvGroup)[ curRow ]->set( mCurT, mCurP, mCurV );

    ui->tableView->setCurrentIndex( m_pTpGroup->index( curRow, 0) );
}
void TpEdit::context_add_below()
{
    on_btnAdd_clicked();
}

//! slots
void TpEdit::on_btnBuild_clicked()
{
//    int ret = buildLine();
//    if ( ret == 0 )
//    {
//        sysLog( tr("Line build completed"), QString::number( mTPs.size()), QString::number( mTVs.size() ) );
//        emit sigLineChanged();
//    }
}

void TpEdit::on_btnDown_clicked()
{
//    if ( !checkChan() )
//    { return; }

//    post_request_prog( msg_download_pvt, pvtEdit, Download );
}
void TpEdit::on_btnStart_clicked()
{
//    if ( !checkChan() )
//    { return; }

//    //! diable start
//    ui->btnStart->setEnabled( false );

//    post_request( msg_start_pvt, pvtEdit, Start );
}
void TpEdit::on_btnStop_clicked()
{
//    if ( !checkChan() )
//    { return; }

//    post_request( msg_stop_pvt, pvtEdit, Stop );
}

//! add below
void TpEdit::on_btnAdd_clicked()
{
    int curRow;

    //! current
    curRow = ui->tableView->currentIndex().row();

//    mCurT += mTStep;
//    mCurP += mPStep;

    Q_ASSERT( NULL != m_pTpGroup );
    m_pTpGroup->insertRow( curRow + 1 );
//    (*mTpvGroup)[ curRow + 1 ]->set( mCurT, mCurP, mCurV );

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
//    //! create the plot
//    if ( NULL != m_pPlot )
//    {}
//    else
//    {
//        m_pPlot = new tpvPlot(this);
//    }

//    if ( NULL == m_pPlot )
//    { return; }

//    updatePlot();

//    m_pPlot->show();
}

void TpEdit::slot_download_cancel()
{
//    //! set model && axesid
//    QString str;
//    int id;
//    str = m_pmcModel->getConnection().getDeviceName();
//    id = m_pmcModel->getConnection().getDeviceCH();

//    MegaDevice::deviceMRQ *pMrq = m_pmcModel->m_pInstMgr->findDevice( str, id );
//    pMrq->terminate( tpvRegion(mAgentAxes,0) );
}

void TpEdit::on_spinLoop_valueChanged(int arg1)
{
//    if ( !checkChan() )
//    { return; }

//    QString str;
//    int axesId;
//    str = m_pmcModel->getConnection().getDeviceName();
//    axesId = m_pmcModel->getConnection().getDeviceCH();

//    MegaDevice::deviceMRQ *pMrq = m_pmcModel->m_pInstMgr->findDevice( str,
//                                                                      axesId );
//    Q_ASSERT( NULL != pMrq );

//    pMrq->setMOTIONPLAN_CYCLENUM( axesId, MRQ_MOTION_SWITCH_1_MAIN, arg1 );
}

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
