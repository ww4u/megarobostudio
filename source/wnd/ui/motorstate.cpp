#include "motorstate.h"
#include "ui_motorstate.h"

MotorState::MotorState(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MotorState)
{
    ui->setupUi(this);

    //! \note combo
    ui->cmbRegion->addItem( tr("MAIN"),0 );
    ui->cmbRegion->addItem( tr("SMALL"),1 );
    for ( int i = 1; i <= 8; i++ )
    {
        ui->cmbRegion->addItem( QString( tr("P%1") ).arg(i), i+1 );
    }

    for ( int i = 0; i < ui->cmbRegion->count(); i++ )
    {
        mStates.insert( 0, tr("IDLE") );
    }

    //! delete on close
    setAttribute( Qt::WA_DeleteOnClose );

    //! clear item
    setName("");
    setState("");
    setProgress( false );
}

MotorState::~MotorState()
{
    delete ui;
}

void MotorState::setName( const QString &name )
{ ui->labelName->setText( name ); }
QString MotorState::name()
{ return ui->labelName->text(); }

void MotorState::setState( const QString &stat, int page )
{
    ui->labelStat->setText( stat );

    if ( mStates.contains( page ) )
    { mStates[page] = stat;  }
    else
    { mStates.insert( page, stat ); }

    ui->cmbRegion->setValue( page );
}
QString MotorState::state( int page )
{
    if ( mStates.contains(page) )
    { return mStates[page]; }
    else
    { return QString(); }
}

void MotorState::setProgress( int mi, int ma, int n )
{
    ui->progressBar->setRange( mi, ma );
    ui->progressBar->setValue( n );
}
void MotorState::setProgress( bool b )
{ ui->progressBar->setVisible( b ); }

void MotorState::on_cmbRegion_activated(int index)
{
    ui->labelStat->setText( mStates[ ui->cmbRegion->value()] );
}


