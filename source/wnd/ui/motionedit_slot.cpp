#include "ui_motionedit.h"
#include "motionedit.h"

#include "../widget/megamessagebox.h"

//! context menu
void motionEdit::context_remove()
{
    on_btnDel_clicked();
}
void motionEdit::context_add_before()
{
    int curRow;

    //! current
    curRow = ui->tableView->currentIndex().row();

    Q_ASSERT( NULL != m_pMotionGroup );
    m_pMotionGroup->insertRow( curRow  );

    ui->tableView->setCurrentIndex( m_pMotionGroup->index( curRow, 0) );
}
void motionEdit::context_add_below()
{
    on_btnAdd_clicked();
}

void motionEdit::context_clear()
{
    on_btnClr_clicked();
}

//! get the hand action options
void motionEdit::slot_robo_changed( const QString &roboName )
{
//    emit sig_robo_changed( roboName );

//    Q_ASSERT( m_pActionDelegate != NULL );
//logDbg();
//    do
//    {
//        VRobot *pRobot = m_pmcModel->m_pInstMgr->findRobot( roboName );
//        if ( NULL == pRobot )
//        { break; }
//logDbg();
//        if ( pRobot->handActions() == NULL )
//        { break; }
//logDbg();
//        handActionModel *pModel = (handActionModel*)pRobot->handActions();
//        if ( NULL == pModel )
//        { break; }
//logDbg();
//        QMap<QString, double> actionMap = pModel->actionMap();
//        if ( actionMap.size() < 1 )
//        { break; }
//logDbg();
//        //! get item
//        QList<QString> strList = actionMap.keys();logDbg()<<QStringList( strList );logDbg();
//        m_pActionDelegate->setItems( QStringList( strList ) );
//        return;
//    }while( 0 );

//    //! no item
//    m_pActionDelegate->setItems( QStringList() );
}

//! update the status by
void motionEdit::slot_joints_trace_changed( )
{
    if ( mJointsPlan.size() > 0 )
    { ui->btnDown->setEnabled(true); }
    else
    { ui->btnDown->setEnabled(false); }

    if ( mJointsPlan.size() > 0 )
    {
        ui->btnGraph->setEnabled(true);

        if ( m_pPlot != NULL && m_pPlot->isVisible() )
        {
            updatePlot();
        }
    }
    else
    { ui->btnGraph->setEnabled(false); }

    ui->btnExport->setEnabled( ui->btnGraph->isEnabled() );
}

//void motionEdit::slot_download_cancel( const QString &name, int id )
//{
//    tpvDownloader::cancelActives();
//}

void motionEdit::on_btnAdd_clicked()
{
    int curRow;

    //! current
    curRow = ui->tableView->currentIndex().row();

    Q_ASSERT( NULL != m_pMotionGroup );
    m_pMotionGroup->insertRow( curRow + 1  );

    ui->tableView->setCurrentIndex( m_pMotionGroup->index( curRow + 1, 0) );
}
void motionEdit::on_btnDel_clicked()
{
    Q_ASSERT( NULL != m_pMotionGroup );
    m_pMotionGroup->removeRow( ui->tableView->currentIndex().row() );
}
void motionEdit::on_btnClr_clicked()
{
    MegaDeleteAffirmMessageBox msgBox;
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    {
        m_pMotionGroup->removeRows( 0, m_pMotionGroup->rowCount(QModelIndex()), QModelIndex() );
    }
}

void motionEdit::on_btnDown_clicked()
{
    post_request( msg_download_motion, motionEdit, Download );
}

void motionEdit::on_btnStart_clicked()
{
    //! connect to robot
    VRobot *pRobot = currentRobot();
    if ( NULL == pRobot )
    { return ; }

    //! download motion
    pRobot->run( );
}

void motionEdit::on_btnStop_clicked()
{
    //! connect to robot
    VRobot *pRobot = currentRobot();
    if ( NULL == pRobot )
    { return; }

    //! download motion
    pRobot->stop( );
}

//! compile
void motionEdit::on_toolButton_clicked()
{
    preCompileTrace();

    int ret;
    ret = compileTrace();

    postCompileTrace( ret );
}

void motionEdit::on_btnPref_clicked()
{
    MotionPref dlg(this);

    dlg.setModal( true );
    dlg.setModel( &mMotionPref );

    dlg.exec();
}

//! \todo some type
void motionEdit::on_btnExport_clicked()
{
    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptSave );
    fDlg.setNameFilter( ("CSV(*.csv)") );
    if ( QDialog::Accepted != fDlg.exec() )
    { return; }

    QFile fileCsv( fDlg.selectedFiles().first() );
    if ( fileCsv.open( QIODevice::WriteOnly ) )
    {}
    else
    { return; }

    QTextStream outStream( &fileCsv );
    outStream.setRealNumberPrecision( 8 );

    //! switch type
    if ( ui->cmbExport->currentIndex() == 0 )       //! joints
    {
        //! title
        outStream<<"t,p1,p2,p3,p4,v1,v2,v3,v4"<<"\n";

        //! items
        for ( int i = 0; i < mJointsPlan.size(); i++ )
        {
            //! t
            outStream<<( mJointsPlan.data()[i].t )<<",";

            for ( int j = 0; j < 4; j++ )
            {
                outStream<<( mJointsPlan.data()[i].p[j] )<<",";
            }

            for ( int j = 0; j < 4; j++ )
            {
                outStream<<( mJointsPlan.data()[i].v[j] )<<",";
            }

            outStream<<"\n";
        }
    }
//    else if ( ui->cmbExport->currentIndex() == 1 )  //! hand
//    {
//        //! title
//        outStream<<"t,p"<<"\n";

//        for ( int i = 0; i < mHandTpvGroup.mItems.size(); i++ )
//        {
//            outStream<<mHandTpvGroup[i]->getT()<<","<<mHandTpvGroup[i]->getP()<<"\n";
//        }
//    }
    else if ( ui->cmbExport->currentIndex() == 1 )  //! xyz
    {
        //! title
        outStream<<"t,x,y,z"<<"\n";

        //! items
        for ( int i = 0; i < mTracePlan.size(); i++ )
        {
            //! t
            outStream<<QString::number( mTracePlan.data()[i].t )<<",";

            outStream<<QString::number( mTracePlan.data()[i].x )<<",";
            outStream<<QString::number( mTracePlan.data()[i].y )<<",";
            outStream<<QString::number( mTracePlan.data()[i].z )<<",";

            outStream<<"\n";
        }
    }
    else
    {}

    fileCsv.close();
}

void motionEdit::on_spinLoop_valueChanged(int arg1)
{
    //! connect to robot
    VRobot *pRobot = currentRobot();
    if ( NULL == pRobot )
    {
        sysError( tr("Invalid robot") );
        return;
    }

    pRobot->setLoop( arg1 );
}
