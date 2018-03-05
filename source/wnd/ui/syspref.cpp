#include "syspref.h"
#include "ui_syspref.h"

#include "../../../include/mcstd.h"

sysPref::sysPref(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sysPref)
{
    ui->setupUi(this);

    ui->cmbSpeed->setValidator( new QIntValidator(125000,1000000,this) );
    ui->cmbTimeout->setValidator( new QIntValidator( time_ms(1),time_s(1),this) );
    ui->cmbInterval->setValidator( new QIntValidator( time_us(100), time_ms(100), this) );

    QIntValidator *pIntValidator = new QIntValidator( 16, 0xfffffff, this );
    ui->cmbGroupFrom->setValidator( pIntValidator );
    ui->cmbGroupTo->setValidator( pIntValidator );
    ui->cmbSendFrom->setValidator( pIntValidator );
    ui->cmbSendTo->setValidator( pIntValidator );
    ui->cmbRecvFrom->setValidator( pIntValidator );
    ui->cmbRecvTo->setValidator( pIntValidator );

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
    ui->cmbPort->setCurrentIndex( mPref.mPort );
    ui->cmbSpeed->setCurrentText( QString::number(mPref.mSpeed) );
    ui->cmbTimeout->setCurrentText( QString::number(mPref.mTimeout) );
    ui->cmbInterval->setCurrentText( QString::number(mPref.mInterval) );
    ui->spinFailTry->setValue( mPref.mFailTryCnt );
    ui->spinEnumTmo->setValue( mPref.mEnumerateTimeout );

    ui->cmbSendFrom->setCurrentText( QString::number(mPref.mSendIdFrom));
    ui->cmbSendTo->setCurrentText( QString::number(mPref.mSendIdTo));

    ui->cmbRecvFrom->setCurrentText( QString::number(mPref.mRecvIdFrom));
    ui->cmbRecvTo->setCurrentText( QString::number(mPref.mRecvIdTo));

    ui->cmbGroupFrom->setCurrentText( QString::number(mPref.mGroupIdFrom));
    ui->cmbGroupTo->setCurrentText( QString::number(mPref.mGroupIdTo));

    ui->dblTimeUnit->setValue( mPref.mTimeUnit );
    ui->dblPosUnit->setValue( mPref.mPosUnit );
    ui->dblVelUnit->setValue( mPref.mVelUnit );

    ui->spinSampleTick->setValue( mPref.mSampleTick );

    ui->chkAutoExpand->setChecked( mPref.mAutoExpand );
    ui->chkAutoLoadSet->setChecked( mPref.mbAutoLoadSetup );
    ui->chkSearch->setChecked( mPref.mbSearchOnOpen );

    //! db
    ui->chkUpload->setChecked( mPref.mDbMeta.mbUpload );
    ui->edtDbName->setText( mPref.mDbMeta.mDbName );
    ui->edtHost->setText( mPref.mDbMeta.mHostName );
    ui->edtTableName->setText( mPref.mDbMeta.mTableName );
    ui->edtUserName->setText( mPref.mDbMeta.mUserName );
    ui->edtPassword->setText( mPref.mDbMeta.mPassword );

    ui->tempPath->setText( mPref.mDumpPath );
}

void sysPref::updateData()
{
    mPref.mPort = ui->cmbPort->currentIndex();
    mPref.mSpeed = ui->cmbSpeed->currentText().toInt();
    mPref.mTimeout = ui->cmbTimeout->currentText().toInt();
    mPref.mInterval = ui->cmbInterval->currentText().toInt();
    mPref.mFailTryCnt = ui->spinFailTry->value();
    mPref.mEnumerateTimeout = ui->spinEnumTmo->value();

    mPref.mSendIdFrom = ui->cmbSendFrom->currentText().toInt();
    mPref.mSendIdTo = ui->cmbSendTo->currentText().toInt();

    mPref.mRecvIdFrom = ui->cmbRecvFrom->currentText().toInt();
    mPref.mRecvIdTo = ui->cmbRecvTo->currentText().toInt();

    mPref.mGroupIdFrom = ui->cmbGroupFrom->currentText().toInt();
    mPref.mGroupIdTo = ui->cmbGroupTo->currentText().toInt();

    mPref.mTimeUnit = ui->dblTimeUnit->value();
    mPref.mPosUnit = ui->dblPosUnit->value();
    mPref.mVelUnit = ui->dblVelUnit->value();

    mPref.mSampleTick = ui->spinSampleTick->value();

    mPref.mAutoExpand = ui->chkAutoExpand->isChecked();
    mPref.mbAutoLoadSetup = ui->chkAutoLoadSet->isChecked();
    mPref.mbSearchOnOpen = ui->chkSearch->isChecked();

    //! db meta
    mPref.mDbMeta.mbUpload = ui->chkUpload->isChecked();
    mPref.mDbMeta.mDbName = ui->edtDbName->text() ;
    mPref.mDbMeta.mHostName = ui->edtHost->text();
    mPref.mDbMeta.mTableName = ui->edtTableName->text();
    mPref.mDbMeta.mUserName = ui->edtUserName->text();
    mPref.mDbMeta.mPassword = ui->edtPassword->text();
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
