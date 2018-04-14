#include "syspref.h"
#include "ui_syspref.h"

#include "../../../include/mcstd.h"

#include <QtSql>

#include "../widget/megamessagebox.h"

sysPref::sysPref(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sysPref)
{
    ui->setupUi(this);

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

    slot_updateValidateEn();
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
    ui->cmbPort->setCurrentIndex( mPref.mPort ); on_cmbPort_currentIndexChanged( mPref.mPort );
    ui->cmbSpeed->setCurrentText( QString::number(mPref.mSpeed) );    
    ui->spinTmo->setValue( mPref.mTimeout );
    ui->spinInterval->setValue( mPref.mInterval );

    ui->spinFailTry->setValue( mPref.mFailTryCnt );
    ui->spinEnumTmo->setValue( mPref.mEnumerateTimeout );
    ui->spinPvtInterval->setValue( mPref.mTpvInterval );
    ui->chkAutoAssignId->setChecked( mPref.mbAutoAssignId );

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
    ui->chkMisaEn->setChecked( mPref.mMisaEn );
    ui->spinMisaSocket->setValue( mPref.mMisaSocket );
    ui->edtRemotePath->setText( mPref.mRemoteDirPath );
logDbg()<<mPref.mMisaEn<<mPref.mMisaSocket;
    ui->tempPath->setText( mPref.mDumpPath );
}

void sysPref::updateData()
{
    mPref.mPort = ui->cmbPort->currentIndex();
    mPref.mSpeed = ui->cmbSpeed->currentText().toInt();
    mPref.mTimeout = ui->spinTmo->value();
    mPref.mInterval = ui->spinInterval->value();
    mPref.mFailTryCnt = ui->spinFailTry->value();
    mPref.mEnumerateTimeout = ui->spinEnumTmo->value();
    mPref.mTpvInterval = ui->spinPvtInterval->value();
    mPref.mbAutoAssignId = ui->chkAutoAssignId->isChecked();

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
    mPref.mRemoteDirPath = ui->edtRemotePath->text();
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

void sysPref::on_buttonBox_clicked(QAbstractButton *button)
{
    if ( ui->buttonBox->buttonRole( button ) == QDialogButtonBox::ResetRole )
    {
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
    if ( index == 0 )
    {
         ui->labelCanPic->setPixmap( QPixmap(QString::fromUtf8(":/res/image/megacan.png")) );
    }
    else if ( index == 1 )
    {
        ui->labelCanPic->setPixmap( QPixmap(QString::fromUtf8(":/res/image/can2.png")) );
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


