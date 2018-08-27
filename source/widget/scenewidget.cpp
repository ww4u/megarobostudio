#include "scenewidget.h"

sceneWidget::sceneWidget( QWidget *parent ) : QLabel( parent )
{
    mbSelected = false;

    m_pBase = NULL;
    m_pModel = NULL;

    mpLabel = new QLineEdit( tr("untitled"), this );
    mpLabel->setSizePolicy( QSizePolicy::Minimum,
                            QSizePolicy::Preferred );

    //! \todo set validator

    connect( mpLabel, SIGNAL(editingFinished()),
             this, SLOT(slot_editingFinished()) );

    setScaledContents( true );

    //! max width
    setMaximumSize( 200,200 );
    mpLabel->setMaximumWidth( 200 );
}

sceneWidget::~sceneWidget()
{
    if ( m_pBase != NULL && m_pBase->getGc() )
    {
        delete m_pBase;
        m_pBase = NULL;
    }
}

void sceneWidget::slot_editingFinished()
{
    emit editingFinished( this, mpLabel->text() );
}

void sceneWidget::setRobot( VRobot *pBase )
{
    Q_ASSERT( NULL != pBase );
    m_pBase = pBase;
    pBase->setGc( true );
    QImage img = pBase->getImage();
    setPixmap( QPixmap::fromImage( img ) );

    QRect rawGeo;
    rawGeo = mpLabel->geometry();

    mpLabel->setGeometry( QRect( rawGeo.x(),
                                 rawGeo.y(),
                                 img.width(),
                                 rawGeo.height() ) );

    setGeometry( QRect(0,0,img.width(),img.height()));
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
        setRobot( (VRobot*)m_pModel );
        setName( pBase->getName() );
    }
    else
    { Q_ASSERT(false); }
}
mcModelObj *sceneWidget::getModelObj()
{ return m_pModel; }

void sceneWidget::setSelected( bool b )
{ mbSelected = b; }
bool sceneWidget::getSelected()
{ return mbSelected; }

void sceneWidget::setName( const QString &name )
{
    mpLabel->setText( name );
}
QString sceneWidget::getName()
{ return mpLabel->text(); }

void sceneWidget::focusOnName()
{ mpLabel->selectAll(); }
