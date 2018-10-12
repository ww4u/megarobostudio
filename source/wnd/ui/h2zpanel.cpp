#include "h2zpanel.h"
#include "ui_h2zpanel.h"

H2ZPanel::H2ZPanel(mcModel *pModel,
                   const QString &roboName,
                   QWidget *parent) :
    RoboPanel(pModel, roboName, parent),
    ui(new Ui::H2ZPanel)
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

H2ZPanel::~H2ZPanel()
{
    delete ui;
}

robotH2Z *H2ZPanel::Robot()
{
    Q_ASSERT( NULL != m_pMcModel );
    VRobot *pRobo;

    pRobo = m_pMcModel->m_pInstMgr->findRobot( windowTitle() );

    robotH2Z *pRealRobo = dynamic_cast<robotH2Z*>( pRobo );
    return pRealRobo;
}

//! move delta
void H2ZPanel::moveD( float dx, float dy, float dz, bool bKeep )
{
    do
    {
        robotH2Z *pRobo = Robot();
        if ( NULL == pRobo )
        { break; }

        if ( bKeep )
        {
            pRobo->move( dx, dy, dz,
                         ui->spinStepT->value(),
                         dx/ui->spinStepT->value(),
                         dy/ui->spinStepT->value(),
                         dz/ui->spinStepT->value(),
                         tpvRegion(0,ui->widget->page())
                         );
        }
        else
        {
            pRobo->move( dx, dy, dz,
                         ui->spinStepT->value(),
                         0, 0, 0,
                         tpvRegion(0,ui->widget->page()) );
        }

    }while( 0 );
}
//! move aim
void H2ZPanel::moveA( float x, float y, float z, float dt )
{
    do
    {
        robotH2Z *pRobo = Robot();
        if ( NULL == pRobo )
        { break; }

        float pos[16];
        int ret;
        ret = pRobo->getPOSE( pos );
        if ( ret != 0 )
        { break; }

        H2ZKeyPointList curve;
        H2ZKeyPoint pt;

        pt.t = 0;
        pt.x = pos[0];
        pt.y = pos[1];
        pt.z = 0;
        curve.append( pt );

        pt.t = dt;
        pt.x = x;
        pt.y = y;
        pt.z = z;
        curve.append( pt );

        pRobo->move( curve, tpvRegion(0,ui->widget->page()) );

    }while( 0 );
}

void H2ZPanel::stop()
{
    robotH2Z *pRobo = Robot();
    if ( NULL == pRobo )
    { return; }

    pRobo->stop( tpvRegion(0,ui->widget->page()) );
}

void H2ZPanel::slot_timeout()
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

void H2ZPanel::on_spinBox_valueChanged(int arg1)
{
    mTimer.setInterval( arg1 );
}

void H2ZPanel::on_chkOnOff_clicked(bool checked)
{
    if ( checked )
    { mTimer.start(); }
    else
    { mTimer.stop(); }
}

void H2ZPanel::on_widget_signal_run_clicked()
{
    robotH2Z *pRobo = Robot();
    if ( NULL == pRobo )
    { return; }

    pRobo->call( 1, tpvRegion(0, ui->widget->page() ) );
}
void H2ZPanel::on_widget_signal_stop_clicked()
{
    robotH2Z *pRobo = Robot();
    if ( NULL == pRobo )
    { return; }

    pRobo->stop( tpvRegion(0, ui->widget->page() ) );
}

void H2ZPanel::on_btnZP_clicked()
{
    if ( ui->chkSingle->isChecked() )
    { moveD( 0, 0, ui->spinStepV->value() ); }
}

void H2ZPanel::on_btnZN_clicked()
{
    if ( ui->chkSingle->isChecked() )
    { moveD( 0, 0, -ui->spinStepV->value() ); }
}

void H2ZPanel::on_btnYN_clicked()
{
    if ( ui->chkSingle->isChecked() )
    { moveD( 0, -ui->spinStepV->value(), 0 ); }
}

void H2ZPanel::on_btnYP_clicked()
{
    if ( ui->chkSingle->isChecked() )
    { moveD( 0, ui->spinStepV->value(), 0 ); }
}

void H2ZPanel::on_btnXP_clicked()
{
    if ( ui->chkSingle->isChecked() )
    { moveD( ui->spinStepV->value(), 0, 0 ); }
}

void H2ZPanel::on_btnXN_clicked()
{
    if ( ui->chkSingle->isChecked() )
    { moveD( -ui->spinStepV->value(), 0, 0 ); }
}

void H2ZPanel::on_btnCenter_clicked()
{
    robotH2Z *pRobo = Robot();
    if ( NULL != pRobo )
    {
        pRobo->goZero( tpvRegion(0,ui->widget->page()) );
    }
}

void H2ZPanel::on_btnGo_clicked()
{
    moveA( ui->spinX->value(),
           ui->spinY->value(),
           ui->spinZ->value(),
           ui->spinT->value() );
}

void H2ZPanel::on_btnMarkNow_clicked()
{
    RpcRequest h2zRpc( RpcRequest::e_req_add_h2z_key_point,
                       RpcRequest::e_type_f_f_f );

    h2zRpc.push( (float)ui->lcdX->value() );
    h2zRpc.push( (float)ui->lcdY->value() );
    h2zRpc.push( 0 );

    sysRpc( h2zRpc );
}

void H2ZPanel::on_btnMarkUser_clicked()
{
    RpcRequest h2zRpc( RpcRequest::e_req_add_h2z_key_point,
                       RpcRequest::e_type_f_f_f );

    h2zRpc.push( (float)ui->spinX->value() );
    h2zRpc.push( (float)ui->spinY->value() );
    h2zRpc.push( (float)ui->spinZ->value() );

    sysRpc( h2zRpc );
}

#define check_press()       if ( ui->chkSingle->isChecked() ) { return; }

#define on_xxx_released()   if ( ui->chkSingle->isChecked() ) { return; }\
                            stop();
void H2ZPanel::on_btnZP_pressed()
{
    check_press();
    moveD( 0,0, ui->spinStepV->value(), true );
}
void H2ZPanel::on_btnZP_released()
{ on_xxx_released(); }
void H2ZPanel::on_btnZN_pressed()
{
    check_press();
    moveD( 0,0, -ui->spinStepV->value(), true );
}
void H2ZPanel::on_btnZN_released()
{ on_xxx_released(); }

void H2ZPanel::on_btnXP_pressed()
{
    check_press();
    moveD( ui->spinStepV->value(),0,0, true );
}
void H2ZPanel::on_btnXP_released()
{ on_xxx_released(); }
void H2ZPanel::on_btnXN_pressed()
{
    check_press();
    moveD( -ui->spinStepV->value(),0,0, true );
}
void H2ZPanel::on_btnXN_released()
{ on_xxx_released(); }

void H2ZPanel::on_btnYP_pressed()
{
    check_press();
    moveD( 0, ui->spinStepV->value(),0, true );
}
void H2ZPanel::on_btnYP_released()
{ on_xxx_released(); }
void H2ZPanel::on_btnYN_pressed()
{
    check_press();
    moveD( 0, -ui->spinStepV->value(),0, true );
}
void H2ZPanel::on_btnYN_released()
{ on_xxx_released(); }
