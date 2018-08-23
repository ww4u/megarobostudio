#include "syspref.h"
#include "ui_syspref.h"

#include "../../../include/mcstd.h"

#include <QtSql>

#include "../widget/megamessagebox.h"

#include <QSerialPortInfo>
#include <QHostInfo>

#ifdef NI_VISA
#include "visa.h"
#endif

sysPref::sysPref(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sysPref)
{
    ui->setupUi(this);

    //! set combox option
    for ( int i = 0; i < ui->cmbPort->count(); i++ )
    { ui->cmbPort->setItemData( i, i ); }

    ui->label_43->setText( tr("Angle") + "(" + QChar(0x00B0) + ")" );

    //! db changed
    connect( ui->edtHost, SIGNAL(textChanged(const QString &)),
             this, SLOT(slot_updateValidateEn()) );
    connect( ui->edtDbName, SIGNAL(textChanged(const QString &)),
             this, SLOT(slot_updateValidateEn()) );
    connect( ui->edtUserName, SIGNAL(textChanged(const QString &)),
             this, SLOT(slot_updateValidateEn()) );
    connect( ui->edtTableName, SIGNAL(textChanged(const QString &)),
             this, SLOT(slot_updateValidateEn()) );
    connect( ui->edtPassword, SIGNAL(textChanged(const QString &)),
             this, SLOT(slot_updateValidateEn()) );

    connect( ui->cmbLang, SIGNAL(activated(int)),
             this, SLOT(slot_styleLang_changed(int) ));
    connect( ui->cmbStyle, SIGNAL(activated(int)),
             this, SLOT(slot_styleLang_changed(int) ));

    m_pCANSetting = ui->tabWidget_2->widget(0);
    m_pMRHTSetting = ui->tabWidget_2->widget(1);
    m_pRs232Setting = ui->tabWidget_2->widget(2);
    m_pUsbSetting = ui->tabWidget_2->widget(3);

    //! validate enable
    ui->portLanSetting->setValidateEnable( true );

    ui->tabWidget_2->removeTab(3);
    ui->tabWidget_2->removeTab(2);
    ui->tabWidget_2->removeTab(1);
    ui->tabWidget_2->removeTab(0);


    //! for com port
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
    foreach( QSerialPortInfo info, portList )
    { ui->cmbComPort->addItem(info.portName()); }

    slot_updateValidateEn();

//    slot_validate_listmrt();

    //! tool bar
    connect( ui->aliasToolBar, SIGNAL(signal_add_clicked()),
             this, SLOT(slot_toolbar_add()));
    connect( ui->aliasToolBar, SIGNAL(signal_del_clicked()),
             this, SLOT(slot_toolbar_del()));
    connect( ui->aliasToolBar, SIGNAL(signal_clr_clicked()),
             this, SLOT(slot_toolbar_clr()));

#ifdef ARCH_32
    ui->cmbPort->removeItem( e_can_mcp );
#endif

#ifdef ARCH_LINUX
    ui->cmbPort->setCurrentIndex( 0 );
    ui->cmbPort->setEnabled( false );
#endif


#ifdef ARCH_RASPBERRY
    ui->cmbPort->setCurrentIndex( e_can_mcp );
    ui->cmbPort->setEnabled( false );

    ui->spinDeviceCount->setVisible( false );
    ui->spinDeviceId->setVisible( false );

    ui->label_34->setVisible( false );
    ui->label_35->setVisible( false );
#endif

}

sysPref::~sysPref()
{
    delete ui;
}

void sysPref::setPref( modelSysPref *pPref )
{
    Q_ASSERT( NULL != pPref );
    m_pPref = pPref;

    updateUi();
}

void sysPref::updateUi()
{
    //! index
    int index = ui->cmbPort->findData( m_pPref->mPort );
    if ( index < 0 )
    { index = 0; }
    ui->cmbPort->setCurrentIndex( index );

#ifdef ARCH_32
    on_cmbPort_currentIndexChanged( ui->cmbPort->currentText() );
#else
#endif

#ifdef ARCH_RASPBERRY
    ui->cmbPort->setEnabled( false );

    ui->spinDeviceCount->setVisible( false );
    ui->spinDeviceId->setVisible( false );

    on_cmbPort_currentIndexChanged( e_can_mcp );
#endif

    ui->cmbSpeed->setCurrentText( QString::number(m_pPref->mSpeed) );

    //! lan
    ui->portLanSetting->setCurrentRsrc( m_pPref->mVisaAddr );
    ui->portLanSetting->setRsrcs( m_pPref->mVisaList );

    ui->port232Setting->setCurrentRsrc( m_pPref->mRs232Addr );
    ui->port232Setting->setRsrcs( m_pPref->mRs232List );

    ui->portUsbSetting->setCurrentRsrc( m_pPref->mUsbAddr );
    ui->portUsbSetting->setRsrcs( m_pPref->mUsbList );

    ui->portLanSetting->setTmo( m_pPref->mVisaTmo );

    //! search options
    //! ASRL[0-9]*::?*INSTR
    //! USB?*INSTR
    ui->portLanSetting->searchOption( "TCPIP?*::?*INSTR");
    ui->port232Setting->searchOption( "ASRL?*::?*INSTR");
    ui->portUsbSetting->searchOption( "USB?*::?*INSTR");

    //! rs232 settting
    uartConfig uCfg;
    uCfg.mBaudInd = m_pPref->mBaudIndex;
    uCfg.mDataInd = m_pPref->mDataWidthIndex;
    uCfg.mParityInd = m_pPref->mParityIndex;
    uCfg.mStopInd = m_pPref->mStopIndex;
    uCfg.mFlowInd = m_pPref->mFlowControlIndex;
    ui->port232Cfg->setConfig( uCfg );

    ui->spinTmo->setValue( m_pPref->mTimeout );
    ui->spinRecvTmo->setValue( m_pPref->mRecvTmo );
    ui->spinInterval->setValue( m_pPref->mInterval );

    ui->spinFailTry->setValue( m_pPref->mFailTryCnt );
    ui->spinEnumTmo->setValue( m_pPref->mEnumerateTimeout );
    ui->spinPvtInterval->setValue( m_pPref->mTpvInterval );
    ui->chkAutoAssignId->setChecked( m_pPref->mbAutoAssignId );

    ui->spinDeviceCount->setValue( m_pPref->mDeviceCount );
    ui->spinDeviceId->setValue( m_pPref->mDeviceId );

    ui->spinSendFrom->setValue((m_pPref->mSendIdFrom));
    ui->spinSendTo->setValue((m_pPref->mSendIdTo));

    ui->spinRecvFrom->setValue((m_pPref->mRecvIdFrom));
    ui->spinRecvTo->setValue((m_pPref->mRecvIdTo));

    ui->spinGpFrom->setValue((m_pPref->mGroupIdFrom));
    ui->spinGpTo->setValue((m_pPref->mGroupIdTo));

    ui->dblTimeUnit->setValue( m_pPref->mTimeUnit );
    ui->dblPosUnit->setValue( m_pPref->mPosUnit );
    ui->dblVelUnit->setValue( m_pPref->mVelUnit );

    ui->spinSampleTick->setValue( m_pPref->mSampleTick );

    ui->chkAutoExpand->setChecked( m_pPref->mAutoExpand );
    ui->chkAutoLoadSet->setChecked( m_pPref->mbAutoLoadSetup );
    ui->chkSearch->setChecked( m_pPref->mbSearchOnOpen );
    ui->chkMaximize->setChecked( m_pPref->mbMaximizeStartup );
    ui->chkShowNotice->setChecked( m_pPref->mbShowNotice );

    ui->chkLoadLast->setChecked( m_pPref->mbAutoLoadPrj );
    ui->chkAutoZeroAffirm->setChecked( m_pPref->mbAffirmZero );
    ui->chkAutoStatus->setChecked( m_pPref->mbAutoStatusView );

    ui->cmbStyle->setCurrentIndex( m_pPref->mStyleIndex );
    ui->cmbLang->setCurrentIndex( m_pPref->mLangIndex );

    //! db
    ui->chkUpload->setChecked( m_pPref->mDbMeta.mbUpload );
    ui->edtDbName->setText( m_pPref->mDbMeta.mDbName );
    ui->edtHost->setText( m_pPref->mDbMeta.mHostName );
    ui->edtTableName->setText( m_pPref->mDbMeta.mTableName );
    ui->edtUserName->setText( m_pPref->mDbMeta.mUserName );
    ui->edtPassword->setText( m_pPref->mDbMeta.mPassword );

    //    slot_updateValidateEn();

    //! misa
    ui->labelHostName->setText( QHostInfo::localHostName() );
    ui->chkMisaEn->setChecked( m_pPref->mMisaEn );
    ui->spinMisaSocket->setValue( m_pPref->mMisaSocket );

    ui->chkComOnOff->setChecked( m_pPref->mComEn );
    ui->cmbComPort->setCurrentText( m_pPref->mComName );

    ui->edtRemotePath->setText( m_pPref->mRemoteDirPath );

    ui->tempPath->setText( m_pPref->mDumpPath );
    ui->edtEventLog->setText( m_pPref->mEventLogFile );

    //! space
    ui->spinDistanceError->setValue( m_pPref->mGeometryResolution );
    ui->spinAngleError->setValue( m_pPref->mAngleResolution );

    //! set model
    ui->tableView->setModel( &m_pPref->mAlias );
}

void sysPref::updateData()
{
//    m_pPref->mPort = ui->cmbPort->currentIndex();

    m_pPref->mPort = ui->cmbPort->currentData().toInt();

    m_pPref->mSpeed = ui->cmbSpeed->currentText().toInt();

    //! lan
    m_pPref->mVisaAddr = ui->portLanSetting->currentRsrc();
    m_pPref->mVisaList = ui->portLanSetting->rsrcs();

    m_pPref->mRs232Addr = ui->port232Setting->currentRsrc();
    m_pPref->mRs232List = ui->port232Setting->rsrcs();

    m_pPref->mUsbAddr = ui->portUsbSetting->currentRsrc();
    m_pPref->mUsbList = ui->portUsbSetting->rsrcs();

    //! rs232 setting
    uartConfig uCfg;
    ui->port232Cfg->getConfig( uCfg );
    m_pPref->mBaudIndex = uCfg.mBaudInd;
    m_pPref->mDataWidthIndex = uCfg.mDataInd;
    m_pPref->mParityIndex = uCfg.mParityInd;
    m_pPref->mStopIndex = uCfg.mStopInd;
    m_pPref->mFlowControlIndex = uCfg.mFlowInd;

    m_pPref->mVisaTmo = ui->portLanSetting->tmo();

    m_pPref->mTimeout = ui->spinTmo->value();
    m_pPref->mRecvTmo = ui->spinRecvTmo->value();
    m_pPref->mInterval = ui->spinInterval->value();
    m_pPref->mFailTryCnt = ui->spinFailTry->value();
    m_pPref->mEnumerateTimeout = ui->spinEnumTmo->value();
    m_pPref->mTpvInterval = ui->spinPvtInterval->value();
    m_pPref->mbAutoAssignId = ui->chkAutoAssignId->isChecked();
    m_pPref->mDeviceCount = ui->spinDeviceCount->value();
    m_pPref->mDeviceId = ui->spinDeviceId->value();

    m_pPref->mSendIdFrom = ui->spinSendFrom->value();
    m_pPref->mSendIdTo = ui->spinSendTo->value();

    m_pPref->mRecvIdFrom = ui->spinRecvFrom->value();
    m_pPref->mRecvIdTo = ui->spinRecvTo->value();

    m_pPref->mGroupIdFrom = ui->spinGpFrom->value();
    m_pPref->mGroupIdTo = ui->spinGpTo->value();

    m_pPref->mTimeUnit = ui->dblTimeUnit->value();
    m_pPref->mPosUnit = ui->dblPosUnit->value();
    m_pPref->mVelUnit = ui->dblVelUnit->value();

    m_pPref->mSampleTick = ui->spinSampleTick->value();

    m_pPref->mAutoExpand = ui->chkAutoExpand->isChecked();
    m_pPref->mbAutoLoadSetup = ui->chkAutoLoadSet->isChecked();
    m_pPref->mbSearchOnOpen = ui->chkSearch->isChecked();
    m_pPref->mbMaximizeStartup = ui->chkMaximize->isChecked();

    m_pPref->mbShowNotice = ui->chkShowNotice->isChecked();

    m_pPref->mbAutoLoadPrj = ui->chkLoadLast->isChecked();
    m_pPref->mbAffirmZero = ui->chkAutoZeroAffirm->isChecked();
    m_pPref->mbAutoStatusView = ui->chkAutoStatus->isChecked();

    m_pPref->mStyleIndex = ui->cmbStyle->currentIndex();
    m_pPref->mLangIndex = ui->cmbLang->currentIndex();

    //! db meta
    m_pPref->mDbMeta.mbUpload = ui->chkUpload->isChecked();
    m_pPref->mDbMeta.mDbName = ui->edtDbName->text() ;
    m_pPref->mDbMeta.mHostName = ui->edtHost->text();
    m_pPref->mDbMeta.mTableName = ui->edtTableName->text();
    m_pPref->mDbMeta.mUserName = ui->edtUserName->text();
    m_pPref->mDbMeta.mPassword = ui->edtPassword->text();

    //! misa
    m_pPref->mMisaEn = ui->chkMisaEn->isChecked();
    m_pPref->mMisaSocket = ui->spinMisaSocket->value();

    m_pPref->mComEn = ui->chkComOnOff->isChecked();
    m_pPref->mComName = ui->cmbComPort->currentText();

    m_pPref->mRemoteDirPath = ui->edtRemotePath->text();

    m_pPref->mEventLogFile = ui->edtEventLog->text();

    //! space
    m_pPref->mGeometryResolution = ui->spinDistanceError->value();
    m_pPref->mAngleResolution = ui->spinAngleError->value();

}

bool sysPref::validateDb()
{
    bool bRet = false;
    do
    {
        //! db
        QSqlDatabase db = QSqlDatabase::addDatabase( "QMYSQL", "verify" );
        if ( !db.isValid() )
        {
            logDbg()<<db.lastError().text();
            return false;
        }

        //! open db
        db.setHostName( ui->edtHost->text() );
        db.setDatabaseName( ui->edtDbName->text() );
        db.setUserName( ui->edtUserName->text() );
        db.setPassword( ui->edtPassword->text() );
        bRet = db.open();
        if ( !bRet )
        {
            logDbg();
            break;
        }

        //! open table
        QString str;
        QSqlQuery query(db);
        str = QString( "SELECT * from %1" ).arg( ui->edtTableName->text() );
        logDbg()<<str;
        bRet = query.exec( str );


    }while( 0 );

    //! remove the database
    QSqlDatabase::removeDatabase( "verify" );

    return bRet;
}

bool sysPref::updateValidateEn()
{
    QLineEdit *lineEdits[]= {
        ui->edtDbName,
        ui->edtHost,
        ui->edtTableName,
        ui->edtUserName,
        ui->edtPassword,
    };

    for ( int i = 0; i < sizeof_array(lineEdits); i++ )
    {
        if ( lineEdits[i]->text().length() > 0 )
        {}
        else
        { return false; }
    }

    return true;
}

//bool sysPref::validateVisaRsrc( QString &strIdn )
//{
//#ifdef NI_VISA
//    ViStatus stat;
//    ViSession viDef, viSes;

//    stat = viOpenDefaultRM( &viDef );
//    if ( stat != VI_SUCCESS )
//    { return false; }

//    QString strRsrc = ui->edtVisa->text();
//    stat = viOpen( viDef, strRsrc.toLatin1().data(), 0, ui->spinVisaTmo->value(), &viSes );
//    if ( stat != VI_SUCCESS )
//    { return false; }

//    //! check idn?
//    stat = viPrintf( viSes, "*IDN?\n" );
//    if ( stat != VI_SUCCESS )
//    { return false; }

//    ViByte buf[128];
//    ViUInt32 retCount;
//    stat = viRead( viSes, buf, 128, &retCount );
//    if ( stat != VI_SUCCESS && stat != VI_SUCCESS_MAX_CNT && stat != VI_SUCCESS_TERM_CHAR )
//    { return false; }

//    if( retCount < 16 )
//    { return false; }

//    strIdn = QString::fromLocal8Bit( (char*)buf, retCount );

//    stat = viClose( viSes );
//    if ( stat != VI_SUCCESS )
//    { return false; }

//    stat = viClose( viDef );
//    if ( stat != VI_SUCCESS )
//    { return false; }
//#endif
//    return true;
//}

void sysPref::on_buttonBox_clicked(QAbstractButton *button)
{
    if ( ui->buttonBox->buttonRole( button ) == QDialogButtonBox::ResetRole )
    {
        if ( QMessageBox::Ok !=
             MegaMessageBox::question( this, QString(tr("Confirm") ),
                                       QString( tr("Confirm to reset") ),
                                       QMessageBox::Ok, QMessageBox::Cancel ) )
        { return; }

        m_pPref->rst();
        updateUi();
    }
    else if ( ui->buttonBox->buttonRole( button ) == QDialogButtonBox::AcceptRole )
    {
        updateData();
    }
}

//#define port_can_mrh_e  "MRH-E"
//#define port_usb_ii     "USB-CAN II"
//#define port_mrh_t      "MRH-T"
//#define port_mrh_u      "MRH-U"

//#define port_mcp        "MCP"
//#define port_rs232      "RS232"
//#define port_usb        "USB"
//#define str_is( )
void sysPref::on_cmbPort_currentIndexChanged(const QString &arg1)
{
    //! remove tab
    for (int i = ui->tabWidget_2->count() - 1; i >= 0 ; i-- )
    { ui->tabWidget_2->removeTab( i ); }

    if ( str_is( arg1, port_can_mrh_e )  )
    {
         ui->labelCanPic->setPixmap( QPixmap(QString::fromUtf8(":/res/image/megacan.png")) );

         ui->tabWidget_2->insertTab( 0, m_pCANSetting, QString( tr("Setting") ) );

         ui->gpAutoCanId->setVisible( true );
    }

    else if ( str_is( arg1, port_usb_ii )  )
    {
        ui->labelCanPic->setPixmap( QPixmap(QString::fromUtf8(":/res/image/can2.png")) );

        ui->tabWidget_2->insertTab( 0, m_pCANSetting, QString( tr("Setting") ) );

        ui->gpAutoCanId->setVisible( true );
    }

    else if ( str_is( arg1, port_mrh_t )  )
    {
        ui->labelCanPic->setPixmap( QPixmap(QString::fromUtf8(":/res/image/mrh-t.png")) );

        ui->tabWidget_2->insertTab( 0, m_pMRHTSetting, QString( tr("Setting") ) );

        ui->gpAutoCanId->setVisible( true );
    }

    else if ( str_is( arg1, port_mrh_u )  )
    {
        ui->labelCanPic->setPixmap( QPixmap(QString::fromUtf8(":/res/image/miniusbcan.png")) );

        ui->tabWidget_2->insertTab( 0, m_pCANSetting, QString( tr("Setting") ) );

        ui->gpAutoCanId->setVisible( true );
    }

    else if ( str_is( arg1, port_mcp )  )
    {
        ui->labelCanPic->setPixmap( QPixmap(QString::fromUtf8(":/res/image/mcp251x.png")) );

        ui->tabWidget_2->insertTab( 0, m_pCANSetting, QString( tr("Setting") ) );

        ui->gpAutoCanId->setVisible( true );
    }

    else if ( str_is( arg1, port_rs232 )  )
    {
        ui->labelCanPic->setPixmap( QPixmap(QString::fromUtf8(":/res/image/rs232.png")) );

        ui->tabWidget_2->insertTab( 0, m_pRs232Setting, QString( tr("Setting") ) );

        ui->gpAutoCanId->setVisible( false );
    }

    else if ( str_is( arg1, port_usb )  )
    {
        ui->labelCanPic->setPixmap( QPixmap(QString::fromUtf8(":/res/image/usb.png")) );

        ui->tabWidget_2->insertTab( 0, m_pUsbSetting, QString( tr("Setting") ) );

        ui->gpAutoCanId->setVisible( true );
    }
    else
    {

    }
}

void sysPref::on_btnDetail_clicked()
{
    QStringList args;
    QString str;
    str = QCoreApplication::applicationDirPath()
            + QDir::separator()
            + QStringLiteral("doc")
            + QDir::separator()
            + QStringLiteral("aggrement.txt");
    str.replace("/","\\");
    args<<str;
    //! \todo linux
    logDbg()<<str;
    QProcess::execute( "explorer.exe", args );
}

void sysPref::on_btnVerify_clicked()
{
    if ( validateDb() )
    {
        QMessageBox::information( this, tr("Success"), tr("Success") );
    }
    else
    {
        QMessageBox::warning( this, tr("Fail"), tr("Fail") );
    }
}

void sysPref::slot_updateValidateEn()
{
    bool bRet = updateValidateEn();

    ui->btnVerify->setEnabled( bRet );
}

void sysPref::slot_styleLang_changed( int index )
{
    MegaMessageBox::information( this, tr("Info"), tr("Setting need be restartd to validate") );
}

//void sysPref::on_btnVerify_2_clicked()
//{
//    QString strIdn;
//    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
//    if ( validateVisaRsrc( strIdn ) )
//    {
//        MegaMessageBox::information( this, tr("Info"), tr("Connect success ") + strIdn );
//        ui->btnAddT->setEnabled( true );
//    }
//    else
//    {
//        MegaMessageBox::warning( this, tr("Info"), tr("Connect fail") );
//        ui->btnAddT->setEnabled( false );
//    }
//    QApplication::restoreOverrideCursor();
//}

//void sysPref::on_edtVisa_textChanged(const QString &arg1)
//{
//    slot_validate_listmrt();
//}

//void sysPref::on_btnAddT_clicked()
//{
//    //! search exist
//    for ( int i = 0; i < ui->listMRTs->count(); i++ )
//    {
//        if ( ui->edtVisa->text().compare( ui->listMRTs->item( i)->text(), Qt::CaseInsensitive ) == 0 )
//        {
//            MegaMessageBox::information( this,
//                                         tr("info"),
//                                         ui->edtVisa->text() + " " + tr("alreay exist") );
//            return;
//        }
//    }

//    //! add item
//    ui->listMRTs->addItem( ui->edtVisa->text() );
//}

//void sysPref::on_btnRemove_clicked()
//{
//    delete ( ui->listMRTs->takeItem( ui->listMRTs->currentRow() ) );
//}

//void sysPref::on_btnClear_clicked()
//{
//    ui->listMRTs->clear();
//}

//void sysPref::on_edtVisa_textEdited(const QString &arg1)
//{
//    slot_validate_listmrt();
//}

//void sysPref::slot_validate_listmrt()
//{
//    if ( ui->edtVisa->text().length() > 0 )
//    {
//        ui->btnVerify_2->setEnabled(true);
//        ui->btnAddT->setEnabled( true );
//    }
//    else
//    {
//        ui->btnVerify_2->setEnabled(false);
//        ui->btnAddT->setEnabled( false );
//    }

//    if ( ui->listMRTs->currentItem() != NULL )
//    { ui->btnRemove->setEnabled( true ); }
//    else
//    { ui->btnRemove->setEnabled( false );}

//    if ( ui->listMRTs->count() > 0 )
//    { ui->btnClear->setEnabled( true ); }
//    else
//    { ui->btnClear->setEnabled( false ); }
//}

//void sysPref::on_listMRTs_currentRowChanged(int currentRow)
//{
//    slot_validate_listmrt();
//}

#define model_      (m_pPref->mAlias)
void sysPref::slot_toolbar_add()
{
    if ( ui->tableView->currentIndex().isValid() )
    { model_.insertRow( ui->tableView->currentIndex().row() + 1 ); }
    else
    { model_.insertRow(0); }
}
void sysPref::slot_toolbar_del()
{
    model_.removeRow( ui->tableView->currentIndex().row() );
}
void sysPref::slot_toolbar_clr()
{
    MegaDeleteAffirmMessageBox msgBox;
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    {
//        model_.removeRows( 0, m_pPref->mAlias.mItems.size() );
        model_.removeRows( 0, model_.mItems.size(), QModelIndex() );
    }
}


