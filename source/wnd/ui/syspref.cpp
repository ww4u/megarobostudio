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

    ui->label_43->setText( tr("Angle") + "(" + char_deg + ")" );

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

    m_pCANSetting = ui->tab_5;
    m_pMRHTSetting = ui->tab_6;
    m_pRs232Setting = ui->tab_7;
    m_pUsbSetting = ui->tab_8;
    m_pESetting = ui->tab_9;

    //! port type
    (ui->portESetting)->setPortType( VCI_MR_USBCAN );
    (ui->portLanSetting)->setPortType( VCI_MR_LANCAN, "TCPIP?*::?*INSTR" );
    (ui->portUsbSetting)->setPortType( VCI_MR_USBTMC, "USB?*::?*INSTR"  );
    ui->port232Setting->setPortType( 0, "ASRL?*::?*INSTR" );

    //! validate enable
    ui->portLanSetting->setValidateEnable( true );
    ui->portLanSetting->setIndicatorAble( true );

    ui->tabWidget_2->removeTab(4);
    ui->tabWidget_2->removeTab(3);
    ui->tabWidget_2->removeTab(2);
    ui->tabWidget_2->removeTab(1);
    ui->tabWidget_2->removeTab(0);

    //! for com port
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
    foreach( QSerialPortInfo info, portList )
    { ui->cmbComPort->addItem(info.portName()); }

    slot_updateValidateEn();

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
    //! only -e
    ui->cmbPort->setCurrentIndex( 0 );
    ui->cmbPort->setEnabled( false );
#endif

#ifdef ARCH_RASPBERRY
    //! only mcp
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
    //! stick the item
#ifdef ARCH_LINUX
    m_pPref->mPort = e_can_mrh_e;
#endif

#ifdef ARCH_RASPBERRY
    m_pPref->mPort = e_can_mcp;
#endif

    //! index
    int index = ui->cmbPort->findData( m_pPref->mPort );
    if ( index < 0 )
    { index = 0; }
    ui->cmbPort->setCurrentIndex( index );

    //! changed
    on_cmbPort_currentIndexChanged( ui->cmbPort->currentText() );

    ui->cmbSpeed->setCurrentText( QString::number(m_pPref->mSpeed) );

    //! lan
    ui->portLanSetting->setCurrentRsrc( m_pPref->mVisaLanAddr );
    ui->portLanSetting->setRsrcs( m_pPref->mVisaLanList );

    ui->port232Setting->setCurrentRsrc( m_pPref->mVisa232Addr );
    ui->port232Setting->setRsrcs( m_pPref->mVisa232List );

    ui->portUsbSetting->setCurrentRsrc( m_pPref->mVisaUsbAddr );
    ui->portUsbSetting->setRsrcs( m_pPref->mVisaUsbList );

    ui->portESetting->setCurrentRsrc( m_pPref->mVisaEAddr );
    ui->portESetting->setRsrcs( m_pPref->mVisaEList );

    //! tmo
    ui->portLanSetting->setTmo( m_pPref->mVisaTmo );
    ui->port232Setting->setTmo( m_pPref->mVisaTmo );
    ui->portUsbSetting->setTmo( m_pPref->mVisaTmo );
    ui->portESetting->setTmo( m_pPref->mVisaTmo );

    //! search options
    //! ASRL[0-9]*::?*INSTR
    //! USB?*INSTR
    ui->portESetting->searchOption("");
//    ui->portLanSetting->searchOption( "TCPIP?*::?*INSTR" );
    ui->portUsbSetting->searchOption( "USB?*::?*INSTR" );
    ui->port232Setting->searchOption( "ASRL?*::?*INSTR" );

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
    ui->chkStopAllOnSearch->setChecked( m_pPref->mbStopOnSearch );

    ui->chkMaximize->setChecked( m_pPref->mbMaximizeStartup );
    ui->chkShowNotice->setChecked( m_pPref->mbShowNotice );
    ui->chkHelp->setChecked( m_pPref->mbShowHelp );
    ui->chkStateInterrupt->setChecked( m_pPref->mbStateInterrupt );

    ui->chkLoadLast->setChecked( m_pPref->mbAutoLoadPrj );
    ui->chkAutoZeroAffirm->setChecked( m_pPref->mbAffirmZero );
    ui->chkAutoStatus->setChecked( m_pPref->mbAutoStatusView );

    ui->cmbStyle->setCurrentIndex( m_pPref->mStyleIndex );
    ui->cmbLang->setCurrentIndex( m_pPref->mLangIndex );

    //! db
    ui->gpDb->setChecked( m_pPref->mDbMeta.mbUpload );
    ui->edtDbName->setText( m_pPref->mDbMeta.mDbName );
    ui->edtHost->setText( m_pPref->mDbMeta.mHostName );
    ui->edtTableName->setText( m_pPref->mDbMeta.mTableName );
    ui->edtUserName->setText( m_pPref->mDbMeta.mUserName );
    ui->edtPassword->setText( m_pPref->mDbMeta.mPassword );

    //! misa
    ui->labelHostName->setText( QHostInfo::localHostName() );
    ui->gpSocket->setChecked( m_pPref->mMisaEn );
    ui->spinMisaSocket->setValue( m_pPref->mMisaSocket );
    ui->spinServerCnt->setValue( m_pPref->mMisaPortCnt );

    ui->gpComPort->setChecked( m_pPref->mComEn );
    ui->cmbComPort->setCurrentText( m_pPref->mComName );

    ui->edtRemotePath->setPlainText( m_pPref->mRemoteDirPath );

    ui->tempPath->setText( m_pPref->mDumpPath );
    ui->edtEventLog->setText( m_pPref->mEventLogFile );

    //! space
    ui->spinDistanceError->setValue( m_pPref->mGeometryResolution );
    ui->spinAngleError->setValue( m_pPref->mAngleResolution );
    ui->chkOmit->setChecked( m_pPref->mOmitEn );
    ui->spinOmit->setValue( m_pPref->mOmitThreshold );

    ui->spinMaxSpeed->setValue( m_pPref->mMaxSpeed );
    ui->spinMaxTerminalSpeed->setValue( m_pPref->mMaxTerminalSpeed );

    ui->labelFont->setText( m_pPref->mFontFamily + ":" + QString::number( m_pPref->mPointSize) );

    //! set model
    ui->gpAlias->setChecked( m_pPref->mAliasEn );
    ui->tableView->setModel( &m_pPref->mAlias );
}

void sysPref::updateData()
{
    m_pPref->mPort = ui->cmbPort->currentData().toInt();

    m_pPref->mSpeed = ui->cmbSpeed->currentText().toInt();

    //! lan
    m_pPref->mVisaLanAddr = ui->portLanSetting->currentRsrc();
    m_pPref->mVisaLanList = ui->portLanSetting->rsrcs();

    m_pPref->mVisa232Addr = ui->port232Setting->currentRsrc();
    m_pPref->mVisa232List = ui->port232Setting->rsrcs();

    m_pPref->mVisaUsbAddr = ui->portUsbSetting->currentRsrc();
    m_pPref->mVisaUsbList = ui->portUsbSetting->rsrcs();

    m_pPref->mVisaEAddr = ui->portESetting->currentRsrc();
    m_pPref->mVisaEList = ui->portESetting->rsrcs();

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
    m_pPref->mbStopOnSearch = ui->chkStopAllOnSearch->isChecked();
    m_pPref->mbMaximizeStartup = ui->chkMaximize->isChecked();

    m_pPref->mbShowNotice = ui->chkShowNotice->isChecked();
    m_pPref->mbShowHelp = ui->chkHelp->isChecked();
    m_pPref->mbStateInterrupt = ui->chkStateInterrupt->isChecked();

    m_pPref->mbAutoLoadPrj = ui->chkLoadLast->isChecked();
    m_pPref->mbAffirmZero = ui->chkAutoZeroAffirm->isChecked();
    m_pPref->mbAutoStatusView = ui->chkAutoStatus->isChecked();

    m_pPref->mStyleIndex = ui->cmbStyle->currentIndex();
    m_pPref->mLangIndex = ui->cmbLang->currentIndex();

    //! db meta
    m_pPref->mDbMeta.mbUpload = ui->gpDb->isChecked();
    m_pPref->mDbMeta.mDbName = ui->edtDbName->text() ;
    m_pPref->mDbMeta.mHostName = ui->edtHost->text();
    m_pPref->mDbMeta.mTableName = ui->edtTableName->text();
    m_pPref->mDbMeta.mUserName = ui->edtUserName->text();
    m_pPref->mDbMeta.mPassword = ui->edtPassword->text();

    //! misa
    m_pPref->mMisaEn = ui->gpSocket->isChecked();
    m_pPref->mMisaSocket = ui->spinMisaSocket->value();
    m_pPref->mMisaPortCnt = ui->spinServerCnt->value();

    m_pPref->mComEn = ui->gpComPort->isChecked();
    m_pPref->mComName = ui->cmbComPort->currentText();

    m_pPref->mRemoteDirPath = ui->edtRemotePath->toPlainText();

    m_pPref->mEventLogFile = ui->edtEventLog->text();

    m_pPref->mAliasEn = ui->gpAlias->isChecked();

    //! space
    m_pPref->mGeometryResolution = ui->spinDistanceError->value();
    m_pPref->mAngleResolution = ui->spinAngleError->value();
    m_pPref->mOmitEn = ui->chkOmit->isChecked();
    m_pPref->mOmitThreshold = ui->spinOmit->value();

    m_pPref->mMaxSpeed = ui->spinMaxSpeed->value();
    m_pPref->mMaxTerminalSpeed = ui->spinMaxTerminalSpeed->value();
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

    if ( ui->gpDb->isChecked() )
    {}
    else
    { return false; }

    return true;
}

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

void sysPref::on_cmbPort_currentIndexChanged(const QString &arg1)
{
    //! remove tab
    for (int i = ui->tabWidget_2->count() - 1; i >= 0 ; i-- )
    { ui->tabWidget_2->removeTab( i ); }

    if ( str_is( arg1, port_can_mrh_e )  )
    {
        ui->labelCanPic->setPixmap( QPixmap(QString::fromUtf8(":/res/image/megacan.png")) );

#ifdef ARCH_32
        ui->tabWidget_2->insertTab( 0, m_pESetting, QString( tr("Setting") ) );
#else
        ui->tabWidget_2->insertTab( 0, m_pCANSetting, QString( tr("Setting") ) );
#endif

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
        model_.removeRows( 0, model_.mItems.size(), QModelIndex() );
    }
}

void sysPref::on_gpAlias_toggled(bool arg1)
{
    ui->aliasToolBar->setEnabled( arg1 );
    ui->tableView->setEnabled( arg1 );
}

void sysPref::on_btnFont_clicked()
{
    QFontDialog dlg;

    if ( dlg.exec() == QDialog::Accepted )
    {
        m_pPref->mFontFamily = dlg.selectedFont().family();
        m_pPref->mPointSize = dlg.selectedFont().pointSize();

        ui->labelFont->setText( m_pPref->mFontFamily + ":" + QString::number( m_pPref->mPointSize) );
    }
}

