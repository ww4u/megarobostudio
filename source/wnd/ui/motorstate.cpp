#include "motorstate.h"
#include "ui_motorstate.h"

MotorState::MotorState(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MotorState)
{
    ui->setupUi(this);

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

void MotorState::setState( const QString &stat )
{ ui->labelStat->setText( stat ); }
QString MotorState::state()
{ return ui->labelStat->text();  }

void MotorState::setProgress( int mi, int ma, int n )
{
    ui->progressBar->setRange( mi, ma );
    ui->progressBar->setValue( n );
}
void MotorState::setProgress( bool b )
{ ui->progressBar->setVisible( b ); }
