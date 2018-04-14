#include "motorstate.h"
#include "ui_motorstate.h"
#include "../../sys/sysapi.h"
#include "../../com/comassist.h"
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

    mbTimerRunning = false;

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

QSize MotorState::sizeHint() const
{ return QSize(400,64); }
//QSize MotorState::minimumSizeHint() const
//{ return sizeHint(); }

void MotorState::setName( const QString &name )
{ ui->labelName->setText( name ); }
QString MotorState::name()
{ return ui->labelName->text(); }

void MotorState::setState( const QString &stat, int page )
{
    ui->labelStat->setText( stat );

    QString strRunning = "RUNNING";

    if ( mStates.contains( page ) )
    {
        if ( stat == strRunning )
        {
            //! enter running
            if ( mStates[page] != strRunning )
            { mRunningTime.start(); }
            //! is running
            else
            {
                ui->labelRunning->setText( comAssist::msToHmsz( mRunningTime.elapsed() ) );
                ui->labelRunning->setVisible( true );
            }
        }
        else
        { ui->labelRunning->setVisible( false );}

        mStates[page] = stat;
    }
    else
    {
        //! enter running
        if ( stat == strRunning  )
        { mRunningTime.start(); }

        ui->labelRunning->setVisible( false );

        mStates.insert( page, stat );
    }

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

    //! guess the end time
    ui->labelRunning->setVisible( true );
    if ( mbTimerRunning )
    {
        if ( n > mLastProg )
        {
            float gTime = ( ma - n ) * mRunningTime.elapsed() / ( n - mLastProg );

            if ( gTime > 0 )
            {
                ui->labelRunning->setText( ( comAssist::msToHmsz(gTime) ) );
            }
        }

        mLastProg = n;
        mRunningTime.restart();
    }
    else
    {
        mLastProg = n;
        mRunningTime.restart();
        mbTimerRunning = true;
        ui->labelRunning->clear();
    }

}
void MotorState::setProgress( bool b )
{
    ui->progressBar->setVisible( b );

    if ( b )
    {}
    else
    { mbTimerRunning = false; }
}

void MotorState::on_cmbRegion_activated(int index)
{
    ui->labelStat->setText( mStates[ ui->cmbRegion->value()] );
}


