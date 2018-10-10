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

    //! for each region
    for ( int i = 0; i < ui->cmbRegion->count(); i++ )
    {
        mStates.insert( 0, tr("IDLE") );

        mRunningTimes.append( new QTime() );

        mTimerRunnings.append( false );
    }

    //! delete on close
    setAttribute( Qt::WA_DeleteOnClose );

    //! clear item
    setName("");
    setState("");
    setProgress( 0, false );
}

MotorState::~MotorState()
{
    delete ui;

    delete_all( mRunningTimes );
}

QSize MotorState::sizeHint() const
{ return QSize(320,64); }

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
        if ( str_is( stat, strRunning) )
        {
            Q_ASSERT( mRunningTimes.at(page) != NULL );

            //! enter running
            if ( mStates[page] != strRunning )
            { mRunningTimes[page]->start(); }
            //! is running
            else
            {
                ui->labelRunning->setText( comAssist::msToHmsz( mRunningTimes[page]->elapsed() ) );
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
        if ( str_is(stat, strRunning) )
        { mRunningTimes[page]->start(); }

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

void MotorState::setProgress( int page, int mi, int ma, int n )
{
    if ( ui->progressBar->maximum() != ma || ui->progressBar->minimum() != mi )
    { ui->progressBar->setRange( mi, ma ); }
    if ( ui->progressBar->value() != n )
    { ui->progressBar->setValue( n ); }

    if ( ui->labelRunning->isHidden() )
    {   //! guess the end time
        ui->labelRunning->setVisible( true );
    }
    if ( mTimerRunnings.at(page) )
    {
        if ( n > mLastProg )
        {
            Q_ASSERT( mRunningTimes.at(page) != NULL );
            float gTime = ( ma - n ) * mRunningTimes.at(page)->elapsed() / ( n - mLastProg );

            if ( gTime > 0 )
            {
                ui->labelRunning->setText( ( comAssist::msToHmsz(gTime) ) );
            }
        }

        mLastProg = n;
        mRunningTimes.at(page)->restart();
    }
    else
    {
        mLastProg = n;
        mRunningTimes.at(page)->restart();
        mTimerRunnings[page] = true;
        if ( ui->labelRunning->text().length() > 0 )
        { ui->labelRunning->clear(); }
    }
}
void MotorState::setProgress( int page, bool b )
{
    ui->progressBar->setVisible( b );

    if ( b )
    {}
    else
    {
        mTimerRunnings[page] = false;
    }
}

void MotorState::on_cmbRegion_activated(int index)
{
    ui->labelStat->setText( mStates[ ui->cmbRegion->value()] );
}


