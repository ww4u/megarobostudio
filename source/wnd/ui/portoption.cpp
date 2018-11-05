#include "portoption.h"
#include "ui_portoption.h"
#include "../../include/mcstd.h"
#include "megamessagebox.h"

#ifdef NI_VISA
#include "visa.h"
#endif

PortOption::PortOption(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PortOption)
{
    ui->setupUi(this);

    mbVerifyAble = false;
    mPortType = 0;

    //! slot
    on_cmbRsrcList_editTextChanged( ui->cmbRsrcList->currentText() );
    on_listMRTs_currentRowChanged( ui->listMRTs->currentRow() );
}

PortOption::~PortOption()
{
    delete ui;
}

void PortOption::setPortType( int tpe )
{ mPortType = tpe; }
int PortOption::portType()
{ return mPortType; }

void PortOption::setValidateEnable( bool b )
{
    mbVerifyAble = b;
    ui->btnVerify_2->setEnabled( b );
}
bool PortOption::validateEnable()
{ return ui->btnVerify_2->isEnabled(); }

void PortOption::setCurrentRsrc( const QString &rsrc )
{
    ui->cmbRsrcList->setCurrentText( rsrc );
}
QString PortOption::currentRsrc()
{
    return ui->cmbRsrcList->currentText();
}

void PortOption::setRsrcs( const QStringList &rsrcs )
{
    ui->listMRTs->clear();
    ui->listMRTs->addItems( rsrcs );

    on_listMRTs_currentRowChanged( 0 );
}
QStringList PortOption::rsrcs()
{
    QStringList rsrcs;
    for ( int i = 0; i < ui->listMRTs->count(); i++ )
    {
        rsrcs.append( ui->listMRTs->item(i)->text() );
    }

    return rsrcs;
}

void PortOption::setTmo( int tmoms )
{
    ui->spinVisaTmo->setValue( tmoms );
}
int PortOption::tmo()
{ return ui->spinVisaTmo->value(); }

//! ASRL[0-9]*::?*INSTR
//! USB?*INSTR
void PortOption::searchRsrc( const QString &rsrcPattern,
                             QStringList &rsrcList )
{
#ifdef NI_VISA    
    if ( mPortType != 0 )
    {
        char descs[1024];
        int ret;
        //! find the device list
        ret = VCI_FindDevice( mPortType, descs, sizeof(descs)-1 );
        if ( ret == 0 )
        { rsrcList.clear(); }
        else
        {
            QString rawStr = QString( descs );
            rsrcList = rawStr.split( ';', QString::SkipEmptyParts );
        }
    }
    else
    {
        ViStatus viStat;
        ViSession viDef;
        viStat = viOpenDefaultRM( &viDef );
        if ( viStat != VI_SUCCESS )
        { return; }

        ViFindList viList;
        ViChar rsrc[64];
        ViUInt32 retCount;
        viStat = viFindRsrc( viDef, rsrcPattern.toLatin1().data(), &viList, &retCount, rsrc  );
        while( viStat == VI_SUCCESS )
        {
            rsrcList.append( rsrc );
            viStat = viFindNext( viList, rsrc );
        }

        viClose( viDef );
    }

#endif
}

void PortOption::searchOption(const QString &rsrcPattern)
{
    QStringList rsrcs;
    searchRsrc( rsrcPattern,
                rsrcs );
    ui->cmbRsrcList->clear();
    ui->cmbRsrcList->addItems( rsrcs );
}

bool PortOption::validateVisaRsrc( QString &strIdn )
{
#ifdef NI_VISA
    ViStatus stat;
    ViSession viDef, viSes;

    stat = viOpenDefaultRM( &viDef );
    if ( stat != VI_SUCCESS )
    { return false; }

    QString strRsrc = ui->cmbRsrcList->currentText();
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

void PortOption::on_btnVerify_2_clicked()
{
    QString strIdn;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    if ( validateVisaRsrc( strIdn ) )
    {
        MegaMessageBox::information( this, tr("Info"), tr("Connect success ") + strIdn );
        ui->btnAddT->setEnabled( true );
    }
    else
    {
        MegaMessageBox::warning( this, tr("Info"), tr("Connect fail") );
        ui->btnAddT->setEnabled( false );
    }
    QApplication::restoreOverrideCursor();
}

void PortOption::on_btnAddT_clicked()
{
    //! search exist
    for ( int i = 0; i < ui->listMRTs->count(); i++ )
    {
        if ( ui->cmbRsrcList->currentText().compare( ui->listMRTs->item( i)->text(), Qt::CaseInsensitive ) == 0 )
        {
            MegaMessageBox::information( this,
                                         tr("info"),
                                         ui->cmbRsrcList->currentText() + " " + tr("alreay exist") );
            return;
        }
    }

    //! add item
    ui->listMRTs->addItem( ui->cmbRsrcList->currentText() );
}

void PortOption::on_btnRemove_clicked()
{
    delete ( ui->listMRTs->takeItem( ui->listMRTs->currentRow() ) );
}

void PortOption::on_btnClear_clicked()
{
    ui->listMRTs->clear();
}

void PortOption::on_cmbRsrcList_editTextChanged(const QString &arg1)
{
    ui->btnVerify_2->setEnabled( arg1.length() > 0 && mbVerifyAble );
    ui->btnAddT->setEnabled( arg1.length() > 0 );
}

void PortOption::on_listMRTs_currentRowChanged(int currentRow)
{
    ui->btnRemove->setEnabled( currentRow >= 0 );
    ui->btnClear->setEnabled( ui->listMRTs->count() > 0 );
}
