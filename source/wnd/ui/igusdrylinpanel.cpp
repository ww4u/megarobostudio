#include "igusdrylinpanel.h"
#include "ui_igusdrylinpanel.h"

IgusDrylinPanel::IgusDrylinPanel( mcModel *pModel,
                                  VRobot *pRobo,
                                  const QString &roboName,
                                  QWidget *parent) :
    RoboPanel(pModel, pRobo, roboName, parent),
    ui(new Ui::IgusDrylinPanel)
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

IgusDrylinPanel::~IgusDrylinPanel()
{
    delete ui;
}

void IgusDrylinPanel::changeEvent( QEvent *event )
{
    RoboPanel::changeEvent( event );

    if ( event->type() == QEvent::LanguageChange )
    { ui->retranslateUi( this ); }
}

robotIgusDelta *IgusDrylinPanel::Robot()
{
    Q_ASSERT( NULL != m_pMcModel );
    VRobot *pRobo;

    pRobo = m_pMcModel->m_pInstMgr->findRobot( windowTitle() );

    robotIgusDelta *pRealRobo = dynamic_cast<robotIgusDelta*>( pRobo );
    return pRealRobo;
}

//! move delta
void IgusDrylinPanel::moveD( float dx, float dy, float dz )
{
    do
    {
        robotIgusDelta *pRobo = Robot();
        if ( NULL == pRobo )
        { break; }

        TraceKeyPoint keyP;
        QList<TraceKeyPoint> curve;

        float pos[3];
        int ret;
        ret = pRobo->getPOSE( pos );
        if ( ret != 0 )
        { break; }

        //! p0
        keyP.t = 0;
        keyP.x = pos[0];
        keyP.y = pos[1];
        keyP.z = pos[2];

        keyP.hand = 0;

        curve.append( keyP );

        //! p1
        keyP.t = ui->spinStepT->value();
        keyP.x = pos[0] + dx;
        keyP.y = pos[1] + dy;
        keyP.z = pos[2] + dz;

        keyP.hand = 0;

        curve.append( keyP );

        pRobo->move( curve, tpvRegion(0,ui->widget->page()) );

    }while( 0 );
}
//! move aim
void IgusDrylinPanel::moveA( float x, float y, float z, float dt )
{
    do
    {
        robotIgusDelta *pRobo = Robot();
        if ( NULL == pRobo )
        { break; }

        float pos[3];
        int ret;
        ret = pRobo->getPOSE( pos );
        if ( ret != 0 )
        { break; }

        TraceKeyPoint keyP;
        QList<TraceKeyPoint> curve;

        //! p0
        keyP.t = 0;
        keyP.x = pos[0];
        keyP.y = pos[1];
        keyP.z = pos[2];
        keyP.hand = 0;

        curve.append( keyP );

        //! p1
        keyP.t = dt;
        keyP.x = x;
        keyP.y = y;
        keyP.z = z;
        keyP.hand = 0;

        curve.append( keyP );

        //! move
        pRobo->move( curve, tpvRegion(0,ui->widget->page()) );

    }while( 0 );
}

void IgusDrylinPanel::slot_timeout()
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
            ui->lcdX->display( (pos[0]) );
            ui->lcdY->display( (pos[1]) );
            ui->lcdZ->display( (pos[2]) );
        }
        else
        {}
    }
}

void IgusDrylinPanel::on_spinBox_valueChanged(int arg1)
{
    mTimer.setInterval( arg1 );
}

void IgusDrylinPanel::on_chkOnOff_clicked(bool checked)
{
    if ( checked )
    { mTimer.start(); }
    else
    { mTimer.stop(); }
}

void IgusDrylinPanel::on_widget_signal_run_clicked()
{
    robotIgusDelta *pRobo = Robot();
    if ( NULL == pRobo )
    { return; }

    pRobo->call( 1, tpvRegion(0, ui->widget->page() ) );
}
void IgusDrylinPanel::on_widget_signal_stop_clicked()
{
    robotIgusDelta *pRobo = Robot();
    if ( NULL == pRobo )
    { return; }

    pRobo->stop( tpvRegion(0, ui->widget->page() ) );
}

void IgusDrylinPanel::on_btnZP_clicked()
{
    moveD( 0, 0, ui->spinStepV->value() );
}

void IgusDrylinPanel::on_btnZN_clicked()
{
    moveD( 0, 0, -ui->spinStepV->value() );
}

void IgusDrylinPanel::on_btnYN_clicked()
{
    moveD( 0, -ui->spinStepV->value(), 0 );
}

void IgusDrylinPanel::on_btnYP_clicked()
{
    moveD( 0, ui->spinStepV->value(), 0 );
}

void IgusDrylinPanel::on_btnXP_clicked()
{
    moveD( ui->spinStepV->value(), 0, 0 );
}

void IgusDrylinPanel::on_btnXN_clicked()
{
    moveD( -ui->spinStepV->value(), 0, 0 );
}

void IgusDrylinPanel::on_btnCenter_clicked()
{
    robotIgusDelta *pRobo = Robot();
    if ( NULL != pRobo )
    {
        pRobo->goZero( tpvRegion(0,ui->widget->page()) );
    }
}

void IgusDrylinPanel::on_btnGo_clicked()
{
    moveA( ui->spinX->value(),
           ui->spinY->value(),
           ui->spinZ->value(),
           ui->spinT->value() );
}

void IgusDrylinPanel::on_btnMarkNow_clicked()
{
    RpcRequest rpc( RpcRequest::e_req_add_t4_key_point,
                       RpcRequest::e_type_f_f_f );

    rpc.push( (float)ui->lcdX->value() );
    rpc.push( (float)ui->lcdY->value() );
    rpc.push( (float)ui->lcdZ->value() );
//    rpc.push( 0 );

    sysRpc( rpc );
}

void IgusDrylinPanel::on_btnMarkUser_clicked()
{
    RpcRequest rpc( RpcRequest::e_req_add_t4_key_point,
                       RpcRequest::e_type_f_f_f );

    rpc.push( (float)ui->spinX->value() );
    rpc.push( (float)ui->spinY->value() );
    rpc.push( (float)ui->spinZ->value() );
//    rpc.push( 0 );

    sysRpc( rpc );
}
