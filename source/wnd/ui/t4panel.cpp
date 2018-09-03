#include "t4panel.h"
#include "ui_t4panel.h"

T4Panel::T4Panel(mcModel *pModel,
                 const QString &roboName,
                 QWidget *parent) :
    RoboPanel( pModel, roboName, parent),
    ui(new Ui::T4Panel)

{
    ui->setupUi(this);

    setWindowTitle( roboName );

    //! timer
    connect( &mTimer, SIGNAL(timeout()),
             this, SLOT(slot_timeout()) );
    mTimer.setInterval( ui->spinBox->value() );

    //! icon
    if ( Robot() != NULL )
    { setWindowIcon( QIcon( QPixmap::fromImage( Robot()->mImage)) ); }

}

T4Panel::~T4Panel()
{
    delete ui;
}

void T4Panel::slot_timeout()
{
    if ( isVisible() )
    {}
    else
    { return; }

    //! update x,y,z
    VRobot *pRobo = Robot();
    if ( pRobo == NULL )
    { return; }

    float pos[16];
    if ( pRobo->poseCount() > 0 )
    {
        if ( pRobo->getPOSE( pos ) == 0 )
        {
            ui->lcdX->display( pos[0] );
            ui->lcdY->display( pos[1] );
            ui->lcdZ->display( pos[2] );
        }
        else
        {}
    }
}

robotSinanju *T4Panel::Robot()
{
    Q_ASSERT( NULL != m_pMcModel );
    VRobot *pRobo;

    pRobo = m_pMcModel->m_pInstMgr->findRobot( windowTitle() );

    robotSinanju *pT4 = dynamic_cast<robotSinanju*>( pRobo );
    return pT4;
}

void T4Panel::moveD( float dx, float dy, float dz )
{
    int ret;

    do
    {
        robotSinanju *pRobo = Robot();
        if ( NULL == pRobo )
        { break; }

        float pos[16];
        ret = pRobo->getPOSE( pos );
        if ( ret != 0 )
        { break; }

        QList<TraceKeyPoint> curve;
        TraceKeyPoint pt;

        pt.t = 0;
        pt.x = pos[0];
        pt.y = pos[1];
        pt.z = pos[2];
        curve.append( pt );

        pt.t = ui->spinStepT->value();
        pt.x += dx;
        pt.y += dy;
        pt.z += dz;
        curve.append( pt );

        pRobo->move( curve, tpvRegion(0, ui->widget->page() ) );

    }while( 0 );
}

void T4Panel::moveA( float x, float y, float z, float dt )
{
    do
    {
        robotSinanju *pRobo = Robot();
        if ( NULL == pRobo )
        { break; }

        float pos[16];
        int ret;
        ret = pRobo->getPOSE( pos );
        if ( ret != 0 )
        { break; }

        QList<TraceKeyPoint> curve;
        TraceKeyPoint pt;

        pt.t = 0;
        pt.x = pos[0];
        pt.y = pos[1];
        pt.z = pos[2];
        curve.append( pt );

        pt.t = dt;
        pt.x = x;
        pt.y = y;
        pt.z = z;
        curve.append( pt );

        pRobo->move( curve, tpvRegion(0,ui->widget->page()) );

    }while( 0 );
}

void T4Panel::on_spinBox_valueChanged(int arg1)
{
    mTimer.setInterval( arg1 );
}

void T4Panel::on_chkOnOff_clicked(bool checked)
{
    if ( checked )
    { mTimer.start(); }
    else
    { mTimer.stop(); }
}

void T4Panel::on_btnYP_clicked()
{
    moveD( 0, ui->spinStepV->value(), 0 );
}

void T4Panel::on_btnYN_clicked()
{
    moveD( 0, -ui->spinStepV->value(), 0 );
}

void T4Panel::on_btnZP_clicked()
{
    moveD( 0, 0, ui->spinStepV->value() );
}

void T4Panel::on_btnZN_clicked()
{
    moveD( 0, 0, -ui->spinStepV->value() );
}

void T4Panel::on_btnXN_clicked()
{
    moveD( -ui->spinStepV->value(),0,0 );
}

void T4Panel::on_btnXP_clicked()
{
    moveD( ui->spinStepV->value(),0,0 );
}

void T4Panel::on_btnCenter_clicked()
{
   robotSinanju *pRobo = Robot();
   if ( NULL != pRobo )
   {
       pRobo->goZero( tpvRegion(0,ui->widget->page()) );
   }
}

void T4Panel::on_btnGo_clicked()
{
    moveA( ui->spinX->value(),
           ui->spinY->value(),
           ui->spinZ->value(),
           ui->spinT->value() );
}

void T4Panel::on_btnMarkNow_clicked()
{
    RpcRequest t4Rpc;

    t4Rpc.push( (float)ui->lcdX->value() );
    t4Rpc.push( (float)ui->lcdY->value() );
    t4Rpc.push( (float)ui->lcdZ->value() );

    sysRpc( t4Rpc );
}

void T4Panel::on_btnMarkUser_clicked()
{
    RpcRequest t4Rpc;

    t4Rpc.push( (float)ui->spinX->value() );
    t4Rpc.push( (float)ui->spinY->value() );
    t4Rpc.push( (float)ui->spinZ->value() );

    sysRpc( t4Rpc );
}


