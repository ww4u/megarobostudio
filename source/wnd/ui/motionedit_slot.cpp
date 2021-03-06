#include "ui_motionedit.h"
#include "motionedit.h"

#include "../widget/megamessagebox.h"

#include "../../com/comassist.h"

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

void motionEdit::slot_timeout()
{
    emit sigSaveRequest( this );
}

void motionEdit::slot_data_changed()
{
    mDlySaveTime.start();

    //! check valid
    bool bValid = m_pMotionGroup->rowCount() >= 2;
    ui->toolButton->setEnabled( bValid );
    ui->btnDown->setEnabled( bValid );
}

//! get the hand action options
void motionEdit::slot_robo_changed( const QString &roboName )
{
    //! \note no roboName
    if ( roboName.isEmpty() )
    { return; }

    VRobot *pRobot;
    if ( checkRobotLive( &pRobot ) )
    {}
    else
    {
        return;
    }

    //! coord able
    ui->coordOption->setVisible( pRobot->coordAble() );
}

//! update the status by
void motionEdit::slot_joints_trace_changed( )
{
//    if ( mJointsTpvGroup.size() > 0 )
//    { ui->btnDown->setEnabled(true); }
//    else
//    { ui->btnDown->setEnabled(false); }

    if ( mJointsTpvGroup.size() > 0 )
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
//    m_pMotionGroup->removeRow( ui->tableView->currentIndex().row() );

    QList<int> selRows;

    //! collect the rows
    QItemSelectionModel *pModel = ui->tableView->selectionModel();
    selRows = comAssist::descRows( pModel );

    foreach( int r, selRows )
    {
        m_pMotionGroup->removeRow( r );
    }
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
    VRobot *pRobot;
    if ( checkRobotLive( &pRobot ) )
    {}
    else
    {
        MegaMessageBox::warning( this, tr("Invalid robot"), tr("Invalid robot")  );
        return;
    }

    //! compile
    int ret = doCompile();
    if ( ret != 0 )
    {
        MegaMessageBox::warning( this, tr("Compile fail"), tr("Compile fail")  );
        return;
    }

    post_request( msg_download_motion, motionEdit, Download );
}

void motionEdit::on_btnStart_clicked()
{
    //! connect to robot
    VRobot *pRobot = currentRobot();
    if ( NULL == pRobot )
    { return ; }

    //! disable
    ui->btnStart->setEnabled( false );

    //! download motion
    pRobot->onLine();
    pRobot->call( tpvRegion(0, m_pmcModel->mConn.roboPage() ) );
}

void motionEdit::on_btnStop_clicked()
{
    //! connect to robot
    VRobot *pRobot = currentRobot();
    if ( NULL == pRobot )
    { return; }

    //! download motion
    pRobot->stop( tpvRegion(0, m_pmcModel->mConn.roboPage() ) );
}

//! compile
void motionEdit::on_toolButton_clicked()
{
    doCompile();
}

void motionEdit::on_btnPref_clicked()
{
    MotionPref dlg(this);

    dlg.setModal( true );
    dlg.setModel( &mMotionPref );

    dlg.exec();
}

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

    int iIndex;
    iIndex = ui->cmbExport->currentIndex();

    //! switch type
    //! joints
    if ( iIndex <= 1 )
    {
        Q_ASSERT( mSectionList.size() >= 2*(iIndex+1) );       //! 0,1

        int columns = mJointsTpvGroup.size();       //! check
        int from, len;

        from = mSectionList.at( iIndex * 2 );
        len = mSectionList.at( iIndex * 2 + 1 );
                                                    //! columns
        Q_ASSERT( from < columns );
        Q_ASSERT( (from + len - 1) < columns );

        //! title
        outStream<<"t";
        for ( int i = from; i < ( from + len ); i++ )
        { outStream<<QString(",p%1").arg(i+1); }
        for ( int i = from; i < ( from + len ); i++ )
        { outStream<<QString(",v%1").arg(i+1); }
        outStream<<CSV_LINE_SEP;

        //! check each size
        int itemCount = mJointsTpvGroup.at( from )->mItems.size();
        for ( int i = ( from + 1 ); i < ( from + len ); i++ )
        {
            Q_ASSERT( mJointsTpvGroup.at(i) != NULL );
            Q_ASSERT( itemCount == mJointsTpvGroup.at(i)->mItems.size() );
        }

        //! foreach item
        for ( int i = 0; i < itemCount; i++ )
        {
            //! t
            outStream<<mJointsTpvGroup[from]->mItems[i]->getT();

            //! p
            for ( int j = from; j < (from+len); j++ )
            {
                outStream<<CSV_COL_SEP<<mJointsTpvGroup[j]->mItems[i]->getP();
            }

            //! v
            for ( int j = from; j < (from+len); j++ )
            {
                outStream<<CSV_COL_SEP<<mJointsTpvGroup[j]->mItems[i]->getV();
            }

            outStream<<CSV_LINE_SEP;
        }
    }
    else if ( ui->cmbExport->currentIndex() == 2 )  //! xyz
    {
        //! title
        outStream<<"t,x,y,z"<<CSV_LINE_SEP;

        //! items
        for ( int i = 0; i < mTracePlan.size(); i++ )
        {
            //! t
            outStream<<QString::number( mTracePlan.data()[i].t );

            outStream<<CSV_COL_SEP<<QString::number( mTracePlan.data()[i].x );
            outStream<<CSV_COL_SEP<<QString::number( mTracePlan.data()[i].y );
            outStream<<CSV_COL_SEP<<QString::number( mTracePlan.data()[i].z );

            outStream<<CSV_LINE_SEP;
        }
    }
    else
    { Q_ASSERT(false); }

    fileCsv.close();
}

void motionEdit::on_spinLoop_valueChanged(int arg1)
{
    VRobot *pRobot;
    if ( checkRobotLive( &pRobot ) )
    {}
    else
    {
        return;
    }

    pRobot->setLoop( arg1,
                     tpvRegion( 0,m_pmcModel->mConn.roboPage()) );
}

void motionEdit::on_btnSmartEdit_toggled(bool checked)
{
    setReqSpy( checked );

    logDbg()<<checked;
}
