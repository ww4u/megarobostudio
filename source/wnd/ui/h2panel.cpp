#include "h2panel.h"
#include "ui_h2panel.h"

H2Panel::H2Panel(mcModel *pModel,
                 const QString &roboName,
                 QWidget *parent) :
    RoboPanel(pModel, roboName, parent),
    ui(new Ui::H2Panel)

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

H2Panel::~H2Panel()
{
    delete ui;
}

robotH2 *H2Panel::Robot()
{
    Q_ASSERT( NULL != m_pMcModel );
    VRobot *pRobo;

    pRobo = m_pMcModel->m_pInstMgr->findRobot( windowTitle() );

    robotH2 *pRealRobo = dynamic_cast<robotH2*>( pRobo );
    return pRealRobo;
}

void H2Panel::moveD( float dx, float dy )
{
    do
    {
        robotH2 *pRobo = Robot();
        if ( NULL == pRobo )
        { break; }

        pRobo->move( dx, dy, ui->spinStepT->value(), 0,0, tpvRegion(0,0));

    }while( 0 );
}

void H2Panel::moveA( float x, float y, float dt )
{
    do
    {
        robotH2 *pRobo = Robot();
        if ( NULL == pRobo )
        { break; }

        float pos[16];
        int ret;
        ret = pRobo->getPOSE( pos );
        if ( ret != 0 )
        { break; }

        H2KeyPointList curve;
        H2KeyPoint pt;

        pt.t = 0;
        pt.x = pos[0];
        pt.y = pos[1];
        curve.append( pt );

        pt.t = dt;
        pt.x = x;
        pt.y = y;
        curve.append( pt );

        pRobo->move( curve, tpvRegion(0,0) );

    }while( 0 );
}

void H2Panel::slot_timeout()
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
        }
        else
        {}
    }
}

void H2Panel::on_spinBox_valueChanged(int arg1)
{
    mTimer.setInterval( arg1 );
}

void H2Panel::on_chkOnOff_clicked(bool checked)
{
    if ( checked )
    { mTimer.start(); }
    else
    { mTimer.stop(); }
}

void H2Panel::on_btnYN_clicked()
{
    moveD( 0, -ui->spinStepV->value() );
}

void H2Panel::on_btnYP_clicked()
{
    moveD( 0, ui->spinStepV->value() );
}

void H2Panel::on_btnXP_clicked()
{
    moveD( ui->spinStepV->value(), 0 );
}

void H2Panel::on_btnXN_clicked()
{
    moveD( -ui->spinStepV->value(), 0 );
}

void H2Panel::on_btnCenter_clicked()
{
    robotH2 *pRobo = Robot();
    if ( NULL != pRobo )
    {
        pRobo->goZero( tpvRegion(0,0) );
    }
}

void H2Panel::on_btnGo_clicked()
{
    moveA( ui->spinX->value(),
           ui->spinY->value(),
           ui->spinT->value() );
}

void H2Panel::on_btnMarkUser_clicked()
{
    RpcRequest h2Rpc( RpcRequest::e_req_add_h2_key_point,
                      RpcRequest::e_type_f_f );

    h2Rpc.push( (float)ui->spinX->value() );
    h2Rpc.push( (float)ui->spinY->value() );

    sysRpc( h2Rpc );
}

void H2Panel::on_btnMarkNow_clicked()
{
    RpcRequest h2Rpc( RpcRequest::e_req_add_h2_key_point,
                      RpcRequest::e_type_f_f );

    h2Rpc.push( (float)ui->lcdX->value() );
    h2Rpc.push( (float)ui->lcdY->value() );

    sysRpc( h2Rpc );
}
