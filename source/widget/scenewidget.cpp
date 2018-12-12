#include "scenewidget.h"

#include "ui_scenewidget.h"

sceneWidget::sceneWidget( QWidget *parent ) : QWidget( parent ), ui(new Ui::sceneWidget)
{
    ui->setupUi(this);

    mbSelected = false;

    m_pBase = NULL;
    m_pModel = NULL;

    ui->mpLabel->setSizePolicy( QSizePolicy::Minimum,
                            QSizePolicy::Preferred );

    connect( ui->mpLabel, SIGNAL(editingFinished()),
             this, SLOT(slot_editingFinished()) );

    ui->image->setScaledContents( true );

    //! max width
//    setMaximumSize( 200,220 );
//    ui->mpLabel->setMaximumWidth( 200 );
}

sceneWidget::~sceneWidget()
{
    if ( m_pBase != NULL && m_pBase->getGc() )
    {
        delete m_pBase;
        m_pBase = NULL;
    }
}

void sceneWidget::paintEvent( QPaintEvent *evt )
{
    if ( mbSelected )
    { }
    else
    { return QWidget::paintEvent( evt); }

    QWidget::paintEvent( evt );

    //! draw border
    QPainter painter( this );

    QPen pen( Qt::DashLine );
    pen.setWidth( 3 );
    pen.setColor( Qt::blue );
    painter.setPen( pen );
    painter.drawRoundRect( rect(), 3, 3 );
}

void sceneWidget::slot_editingFinished()
{
    emit editingFinished( this, ui->mpLabel->text() );
}

void sceneWidget::attachRobot( VRobot *pBase )
{logDbg();
    Q_ASSERT( NULL != pBase );
    m_pBase = pBase;
//    pBase->setGc( true );
    QImage img = pBase->getImage();
    ui->image->setPixmap( QPixmap::fromImage( img ) );
}
VRobot *sceneWidget::getRobot()
{
    return m_pBase;
}

void sceneWidget::setModelObj( mcModelObj *pBase )
{
    Q_ASSERT( NULL != pBase );
    m_pModel = pBase;

    //! model
    if ( m_pModel->getType() == mcModelObj::model_scene_variable )
    {
        attachRobot( (VRobot*)m_pModel );
        setName( pBase->getName() );
    }
    else
    { Q_ASSERT(false); }
}
mcModelObj *sceneWidget::getModelObj()
{ return m_pModel; }

bool sceneWidget::matchWidget( QLabel *plabel )
{
    if ( plabel == ui->image )
    { return true; }
    else
    { return false; }
}

void sceneWidget::setSelected( bool b )
{
    mbSelected = b;

    update();
}
bool sceneWidget::getSelected()
{ return mbSelected; }

void sceneWidget::setName( const QString &name )
{
    mName = name;
    ui->mpLabel->setText( name );
}
QString sceneWidget::getName()
{ return ui->mpLabel->text(); }

QString sceneWidget::getShadowName()
{ return mName; }

void sceneWidget::focusOnName()
{ ui->mpLabel->selectAll(); }

void sceneWidget::on_mpLabel_textChanged(const QString &arg1)
{
    Q_ASSERT( NULL != m_pModel );
    if ( arg1.length() > 0 )
    { m_pModel->setName( arg1 ); }
}

void sceneWidget::on_mpLabel_textEdited(const QString &arg1)
{
    emit editingFinished( this, arg1 );
}
