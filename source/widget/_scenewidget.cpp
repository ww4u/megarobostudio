#include "scenewidget.h"
#include "ui_scenewidget.h"

sceneWidget::sceneWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::sceneWidget)
{
    ui->setupUi(this);

    mbSelected = false;

    m_pBase = NULL;
    m_pModel = NULL;
}

sceneWidget::~sceneWidget()
{
    delete ui;
}

void sceneWidget::setPixmap(const QPixmap &img )
{
    ui->label->setPixmap( img );
}

void sceneWidget::setName( const QString &name )
{
    ui->label->setText( name );
}
QString sceneWidget::getName()
{ return ui->label->text(); }

void sceneWidget::setRobot( robotBase *pBase )
{
    Q_ASSERT( NULL != pBase );
    m_pBase = pBase;
}
robotBase *sceneWidget::getRobot()
{
    return m_pBase;
}

void sceneWidget::setModelObj( mcModelObj *pBase )
{
    Q_ASSERT( NULL != pBase );
    m_pModel = pBase;

    //! model
    if ( m_pModel->getType() == mcModelObj::model_scene_variable )
    { setRobot( (robotBase*)m_pModel->getObj() ); }
    else
    { Q_ASSERT(false); }
}
mcModelObj *sceneWidget::getModelObj()
{ return m_pModel; }

void sceneWidget::setSelected( bool b )
{ mbSelected = b; qDebug()<<__FUNCTION__<<__LINE__<<b; }
bool sceneWidget::getSelected()
{ qDebug()<<__FUNCTION__<<__LINE__<<mbSelected;
            return mbSelected; }
