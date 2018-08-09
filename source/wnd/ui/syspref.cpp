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
    ui->tabWidget_2->removeTab(1);
    ui->tabWidget_2->removeTab(0);

    //! for com port
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
    foreach( QSerialPortInfo info, portList )
    { ui->cmbComPort->addItem(info.portName()); }

    slot_updateValidateEn();
    slot_validate_listmrt();

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

void sysPref::setPref( const modelSysPref &pref )
{
    mPref = pref;

    updateUi();
}

modelSysPref sysPref::getPref()
{
    return mPref;
}

void sysPref::updateUi()
{

#ifdef ARCH_32
    ui->cmbPort->setCurrentIndex( mPref.mPort );
    on_cmbPort_currentIndexChanged( mPref.mPort );
#else
#endif

#ifdef ARCH_RASPBERRY
    ui->cmbPort->setCurrentIndex( e_can_mcp );
    ui->cmbPort->setEnabled( false );

    ui->spinDeviceCount->setVisible( false );
    ui->spinDeviceId->setVisible( false );

    on_cmbPort_currentIndexChanged( e_can_mcp );
#endif


    ui->cmbSpeed->setCurrentText( QString::number(mPref.mSpeed) );

    ui->edtVisa->setText( mPref.mVisaAddr );
    ui->spinVisaTmo->setValue( mPref.mVisaTmo );
    ui->listMRTs->clear();
    ui->listMRTs->addItems( mPref.mVisaList );

    ui->spinTmo->setValue( mPref.mTimeout );
    ui->spinRecvTmo->setValue( mPref.mRecvTmo );
    ui->spinInterval->setValue( mPref.mInterval );

    ui->spinFailTry->setValue( mPref.mFailTryCnt );
    ui->spinEnumTmo->setValue( mPref.mEnumerateTimeout );
    ui->spinPvtInterval->setValue( mPref.mTpvInterval );
    ui->chkAutoAssignId->setChecked( mPref.mbAutoAssignId );

    ui->spinDeviceCount->setValue( mPref.mDeviceCount );
    ui->spinDeviceId->setValue( mPref.mDeviceId );

    ui->spinSendFrom->setValue((mPref.mSendIdFrom));
    ui->spinSendTo->setValue((mPref.mSendIdTo));

    ui->spinRecvFrom->setValue((mPref.mRecvIdFrom));
    ui->spinRecvTo->setValue((mPref.mRecvIdTo));

    ui->spinGpFrom->setValue((mPref.mGroupIdFrom));
    ui->spinGpTo->setValue((mPref.mGroupIdTo));

    ui->dblTimeUnit->setValue( mPref.mTimeUnit );
    ui->dblPosUnit->setValue( mPref.mPosUnit );
    ui->dblVelUnit->setValue( mPref.mVelUnit );

    ui->spinSampleTick->setValue( mPref.mSampleTick );

    ui->chkAutoExpand->setChecked( mPref.mAutoExpand );
    ui->chkAutoLoadSet->setChecked( mPref.mbAutoLoadSetup );
    ui->chkSearch->setChecked( mPref.mbSearchOnOpen );
    ui->chkMaximize->setChecked( mPref.mbMaximizeStartup );

    ui->chkLoadLast->setChecked( mPref.mbAutoLoadPrj );
    ui->chkAutoZeroAffirm->setChecked( mPref.mbAffirmZero );
    ui->chkAutoStatus->setChecked( mPref.mbAutoStatusView );

    ui->cmbStyle->setCurrentIndex( mPref.mStyleIndex );
    ui->cmbLang->setCurrentIndex( mPref.mLangIndex );

    //! db
    ui->chkUpload->setChecked( mPref.mDbMeta.mbUpload );
    ui->edtDbName->setText( mPref.mDbMeta.mDbName );
    ui->edtHost->setText( mPref.mDbMeta.mHostName );
    ui->edtTableName->setText( mPref.mDbMeta.mTableName );
    ui->edtUserName->setText( mPref.mDbMeta.mUserName );
    ui->edtPassword->setText( mPref.mDbMeta.mPassword );

    //    slot_updateValidateEn();

    //! misa
    ui->labelHostName->setText( QHostInfo::localHostName() );
    ui->chkMisaEn->setChecked( mPref.mMisaEn );
    ui->spinMisaSocket->setValue( mPref.mMisaSocket );

    ui->chkComOnOff->setChecked( mPref.mComEn );
    ui->cmbComPort->setCurrentText( mPref.mComName );

    ui->edtRemotePath->setText( mPref.mRemoteDirPath );

    ui->tempPath->setText( mPref.mDumpPath );
    ui->edtEventLog->setText( mPref.mEventLogFile );

    //! space
    ui->spinDistanceError->setValue( mPref.mGeometryResolution );
    ui->spinAngleError->setValue( mPref.mAngleResolution );

    //! sn list
    QTableWidgetItem *pItem;
//    for( int i = 0; i < ui->tableAlias->rowCount(); i++ )
    for ( int i = 0; i < 8; i++ )
    {logDbg()<<i<<ui->tableAlias->rowCount()<<ui->tableAlias->columnCount();
        pItem = ui->tableAlias->takeItem( i, 0 );
        if ( pItem == NULL )
        {
            pItem = new QTableWidgetItem( mPref.mSNList.at(i) );
        }
        else
        {
            logDbg()<<pItem->data(Qt::DisplayRole).toString();
            pItem->setData( Qt::EditRole, mPref.mSNList.at(i) );
        }
        ui->tableAlias->setItem( i, 0, pItem );

        pItem = ui->tableAlias->takeItem( i, 1 );
        if ( pItem == NULL )
        {
            pItem = new QTableWidgetItem( mPref.mAliases.at(i) );
        }
        else
        {
            logDbg()<<pItem->data(Qt::DisplayRole).toString();
            pItem->setData( Qt::EditRole, mPref.mAliases.at(i) );
        }

        ui->tableAlias->setItem( i, 1, pItem );
    }
}

void sysPref::updateData()
{
    mPref.mPort = ui->cmbPort->currentIndex();
    mPref.mSpeed = ui->cmbSpeed->currentText().toInt();

    mPref.mVisaAddr = ui->edtVisa->text();
    mPref.mVisaTmo = ui->spinVisaTmo->value();
    mPref.mVisaList.clear();
    for ( int i = 0; i < ui->listMRTs->count(); i++ )
    { mPref.mVisaList.append( ui->listMRTs->item(i)->text() ); }

    mPref.mTimeout = ui->spinTmo->value();
    mPref.mRecvTmo = ui->spinRecvTmo->value();
    mPref.mInterval = ui->spinInterval->value();
    mPref.mFailTryCnt = ui->spinFailTry->value();
    mPref.mEnumerateTimeout = ui->spinEnumTmo->value();
    mPref.mTpvInterval = ui->spinPvtInterval->value();
    mPref.mbAutoAssignId = ui->chkAutoAssignId->isChecked();
    mPref.mDeviceCount = ui->spinDeviceCount->value();
    mPref.mDeviceId = ui->spinDeviceId->value();

    mPref.mSendIdFrom = ui->spinSendFrom->value();
    mPref.mSendIdTo = ui->spinSendTo->value();

    mPref.mRecvIdFrom = ui->spinRecvFrom->value();
    mPref.mRecvIdTo = ui->spinRecvTo->value();

    mPref.mGroupIdFrom = ui->spinGpFrom->value();
    mPref.mGroupIdTo = ui->spinGpTo->value();

    mPref.mTimeUnit = ui->dblTimeUnit->value();
    mPref.mPosUnit = ui->dblPosUnit->value();
    mPref.mVelUnit = ui->dblVelUnit->value();

    mPref.mSampleTick = ui->spinSampleTick->value();

    mPref.mAutoExpand = ui->chkAutoExpand->isChecked();
    mPref.mbAutoLoadSetup = ui->chkAutoLoadSet->isChecked();
    mPref.mbSearchOnOpen = ui->chkSearch->isChecked();
    mPref.mbMaximizeStartup = ui->chkMaximize->isChecked();

    mPref.mbAutoLoadPrj = ui->chkLoadLast->isChecked();
    mPref.mbAffirmZero = ui->chkAutoZeroAffirm->isChecked();
    mPref.mbAutoStatusView = ui->chkAutoStatus->isChecked();

    mPref.mStyleIndex = ui->cmbStyle->currentIndex();
    mPref.mLangIndex = ui->cmbLang->currentIndex();

    //! db meta
    mPref.mDbMeta.mbUpload = ui->chkUpload->isChecked();
    mPref.mDbMeta.mDbName = ui->edtDbName->text() ;
    mPref.mDbMeta.mHostName = ui->edtHost->text();
    mPref.mDbMeta.mTableName = ui->edtTableName->text();
    mPref.mDbMeta.mUserName = ui->edtUserName->text();
    mPref.mDbMeta.mPassword = ui->edtPassword->text();

    //! misa
    mPref.mMisaEn = ui->chkMisaEn->isChecked();
    mPref.mMisaSocket = ui->spinMisaSocket->value();

    mPref.mComEn = ui->chkComOnOff->isChecked();
    mPref.mComName = ui->cmbComPort->currentText();

    mPref.mRemoteDirPath = ui->edtRemotePath->text();

    mPref.mEventLogFile = ui->edtEventLog->text();

    //! space
    mPref.mGeometryResolution = ui->spinDistanceError->value();
    mPref.mAngleResolution = ui->spinAngleError->value();

    //! alias list
    QString str;
    for( int i = 0; i < ui->tableAlias->rowCount(); i++ )
    {
//        for( int j = 0; j < ui->tableAlias->columnCount(); j++ )
        {
            str = ui->tableAlias->item( i, 0 )->data( Qt::DisplayRole ).toString();
            mPref.mSNList[i] = str;

            str = ui->tableAlias->item( i, 1 )->data( Qt::DisplayRole ).toString();
            mPref.mAliases[i] = str;
        }
//        ui->tableAlias->item()
    }
//    mPref.mAliases
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

bool sysPref::validateVisaRsrc( QString &strIdn )
{
#ifdef NI_VISA
    ViStatus stat;
    ViSession viDef, viSes;

    stat = viOpenDefaultRM( &viDef );
    if ( stat != VI_SUCCESS )
    { return false; }

    QString strRsrc = ui->edtVisa->text();
    stat = viOpen( viDef, strRsrc.toLatin1().data(), 0, ui->spinVisaTmo->value(), &viSes );
    if ( stat != VI_SUCCESS )
    { return false; }

    //! check idn?
    stat = viPrintf( viSes, "*IDN?\n" );
    if ( stat != VI_SUCCESS )
    { return false; }

    ViByte buf[128];
    ViUInt32 retCount;
    stat = viRead( viSes, buf, 128, &retCount );
    if ( stat != VI_SUCCESS && stat != VI_SUCCESS_MAX_CNT && stat != VI_SUCCESS_TERM_CHAR )
    { return false; }

    if( retCount < 16 )
    { return false; }

    strIdn = QString::fromLocal8Bit( (char*)buf, retCount );

    stat = viClose( viSes );
    if ( stat != VI_SUCCESS )
    { return false; }

    stat = viClose( viDef );
    if ( stat != VI_SUCCESS )
    { return false; }
#endif
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

        mPref.rst();
        updateUi();
    }
    else if ( ui->buttonBox->buttonRole( button ) == QDialogButtonBox::AcceptRole )
    {
        updateData();
    }
}

void sysPref::on_cmbPort_currentIndexChanged(int index)
{
    //! remove tab
    for (int i = ui->tabWidget_2->count() - 1; i >= 0 ; i-- )
    { ui->tabWidget_2->removeTab( i ); }

    if ( index == e_can_mrh_e )
    {
         ui->labelCanPic->setPixmap( QPixmap(QString::fromUtf8(":/res/image/megacan.png")) );

         ui->tabWidget_2->insertTab( 0, m_pCANSetting, QString( tr("Setting") ) );
    }
    else if ( index == e_can_usb_ii )
    {
        ui->labelCanPic->setPixmap( QPixmap(QString::fromUtf8(":/res/image/can2.png")) );

        ui->tabWidget_2->insertTab( 0, m_pCANSetting, QString( tr("Setting") ) );
    }
    else if ( index == e_can_mrh_t )
    {
        ui->labelCanPic->setPixmap( QPixmap(QString::fromUtf8(":/res/image/mrh-t.png")) );

        ui->tabWidget_2->insertTab( 0, m_pMRHTSetting, QString( tr("Setting") ) );
    }
    else if ( index == e_can_mrh_u )
    {
        ui->labelCanPic->setPixmap( QPixmap(QString::fromUtf8(":/res/image/miniusbcan.png")) );

        ui->tabWidget_2->insertTab( 0, m_pCANSetting, QString( tr("Setting") ) );
    }
    else if ( index == e_can_mcp )
    {
        ui->labelCanPic->setPixmap( QPixmap(QString::fromUtf8(":/res/image/mcp251x.png")) );

        ui->tabWidget_2->insertTab( 0, m_pCANSetting, QString( tr("Setting") ) );
    }
    else
    {}
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

void sysPref::on_btnVerify_2_clicked()
{
    QString strIdn;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    if ( validateVisaRsrc( strIdn ) )
    {
        MegaMessageBox::information( this, tr("Info"), tr("Connect success ") + strIdn );
        ui->btnAdd->setEnabled( true );
    }
    else
    {
        MegaMessageBox::warning( this, tr("Info"), tr("Connect fail") );
        ui->btnAdd->setEnabled( false );
    }
    QApplication::restoreOverrideCursor();
}

void sysPref::on_edtVisa_textChanged(const QString &arg1)
{
    slot_validate_listmrt();
}

void sysPref::on_btnAdd_clicked()
{
    //! search exist
    for ( int i = 0; i < ui->listMRTs->count(); i++ )
    {
        if ( ui->edtVisa->text().compare( ui->listMRTs->item( i)->text(), Qt::CaseInsensitive ) == 0 )
        {
            MegaMessageBox::information( this,
                                         tr("info"),
                                         ui->edtVisa->text() + " " + tr("alreay exist") );
            return;
        }
    }

    //! add item
    ui->listMRTs->addItem( ui->edtVisa->text() );
}

void sysPref::on_btnRemove_clicked()
{
    delete ( ui->listMRTs->takeItem( ui->listMRTs->currentRow() ) );
}

void sysPref::on_btnClear_clicked()
{
    ui->listMRTs->clear();
}

void sysPref::on_edtVisa_textEdited(const QString &arg1)
{
    slot_validate_listmrt();
}

void sysPref::slot_validate_listmrt()
{
    if ( ui->edtVisa->text().length() > 0 )
    {
        ui->btnVerify_2->setEnabled(true);
        ui->btnAdd->setEnabled( true );
    }
    else
    {
        ui->btnVerify_2->setEnabled(false);
        ui->btnAdd->setEnabled( false );
    }

    if ( ui->listMRTs->currentItem() != NULL )
    { ui->btnRemove->setEnabled( true ); }
    else
    { ui->btnRemove->setEnabled( false );}

    if ( ui->listMRTs->count() > 0 )
    { ui->btnClear->setEnabled( true ); }
    else
    { ui->btnClear->setEnabled( false ); }
}

void sysPref::on_listMRTs_currentRowChanged(int currentRow)
{
    slot_validate_listmrt();
}
