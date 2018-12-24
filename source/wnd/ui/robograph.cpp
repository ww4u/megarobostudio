#include "robograph.h"
#include "ui_robograph.h"

#include "../model/mrggraphmodel.h"

#include "../widget/lineentity.h"
#include "../widget/rectentity.h"
#include "../widget/imageentity.h"

RoboGraph::RoboGraph(QWidget *parent) :
    modelView(parent),
    ui(new Ui::RoboGraph)
{
    ui->setupUi(this);

    //! debug
    m_pLineWidget = new LineEntity( this );
    m_pLineWidget->setVisible( true );

    m_pRectWidget = new RectEntity(this);
    m_pRectWidget->setVisible( true );

    m_pRectWidget->setGeometry( 200,200,100,50);

    mChildWidgets.append( m_pLineWidget );
    mChildWidgets.append( m_pRectWidget );

    EntityWidget *pWig;
    pWig = new ImageEntity( this );
    pWig->setGeometry( 300,10,100,100 );
    mChildWidgets.append( pWig );
}

RoboGraph::~RoboGraph()
{
    delete ui;
}

void RoboGraph::mousePressEvent(QMouseEvent *event)
{
    //! save now
    mPtMouse = event->pos();

    //! selected
    QRect wigRect;
    for ( int i = 0; i < mChildWidgets.size(); i++ )
    {
        wigRect = mChildWidgets.at(i)->geometry();
        if ( wigRect.contains( mPtMouse ) )
        { mChildWidgets[i]->setSelected( true ); }
        else
        { mChildWidgets[i]->setSelected( false ); }
    }

    modelView::mousePressEvent( event );
}
void RoboGraph::mouseReleaseEvent( QMouseEvent *event )
{
    modelView::mouseReleaseEvent( event );
}
void RoboGraph::mouseMoveEvent(QMouseEvent *event)
{
    QPoint ptNow = event->pos();

    //! move
    int dx, dy, x, y;
    dx = ptNow.x() - mPtMouse.x();
    dy = ptNow.y() - mPtMouse.y();

    QRect wigRect;
    for ( int i = 0; i < mChildWidgets.size(); i++ )
    {
        if ( mChildWidgets.at(i)->selected() )
        {
            wigRect = mChildWidgets.at(i)->geometry();

            x = wigRect.x() + dx;
            y = wigRect.y() + dy;
            x = qMax( x, 0 );
            y = qMax( y, 0 );
            mChildWidgets[i]->move( x, y );
        }
    }

    //! save now
    mPtMouse = ptNow;

    modelView::mouseMoveEvent( event );
}

void RoboGraph::dragEnterEvent(QDragEnterEvent *event)
{
    logDbg()<<event->mimeData()->formats();

    if (event->mimeData()->hasFormat("devicemgr/robot"))
    { event->acceptProposedAction(); }
    else if ( event->mimeData()->hasFormat("oplib/operator") )
    { event->acceptProposedAction(); }
    else
    {}
}
void RoboGraph::dropEvent(QDropEvent *event)
{
    //! point
    QPoint pt;
    pt = mapFrom( this, event->pos() );

    //! robot
    if (event->mimeData()->hasFormat("devicemgr/robot"))
    {
        logDbg()<<event->mimeData()->data("devicemgr/robot");

        event->acceptProposedAction();


        //! add
        EntityWidget *pWig = addRoboEntity(  event->mimeData()->data("devicemgr/robot"),
                                             event->mimeData()->text() );
        if ( NULL == pWig )
        { return; }

        pWig->move( pt );
        pWig->setVisible( true );
    }
    //! operator
    else if ( event->mimeData()->hasFormat("oplib/operator") )
    {
        event->acceptProposedAction();

        EntityWidget *pWig = addOperatorEntity( event->mimeData()->text(),
                                                event->mimeData()->data( "oplib/operator") );

        if ( NULL == pWig )
        { return; }

        pWig->move( pt );
        pWig->setVisible( true );
    }
    else
    {

    }
}

int RoboGraph::save( QString &outFileName )
{
    //! get obj

    MrgGraphModel *pModel = dynamic_cast< MrgGraphModel*>( m_pModelObj );
    if ( NULL == pModel )
    { logDbg(); return -1; }

    //! build name
    outFileName = m_pModelObj->getPath() + QDir::separator() + m_pModelObj->getName();
    outFileName = QDir::toNativeSeparators( outFileName );

    return pModel->save( outFileName );
}
int RoboGraph::saveAs( QString &name )
{
    return save( name );
}

EntityWidget* RoboGraph::addRoboEntity( const QString &className, const QString &name )
{
    //! create a new robot
    VRobot *pNewRobot = robotFact::createRobot( className );
    Q_ASSERT( NULL != pNewRobot );

    ImageEntity *pWig;
    pWig = new ImageEntity( this );
    if ( NULL == pWig )
    { return NULL; }

    mChildWidgets.append( pWig );

    pWig->setLabel( name );
    pWig->setImage( pNewRobot->getImage() );

    delete pNewRobot;

    return pWig;
}

EntityWidget* RoboGraph::addOperatorEntity( const QString &className, const QString &icon )
{
    ImageEntity *pWig = new ImageEntity( this );

    if ( NULL == pWig )
    { return NULL; }

    mChildWidgets.append( pWig );

    pWig->setLabel( className );
    pWig->setImage( QImage( icon ) );

    return pWig;
}
