#include "robograph.h"
#include "ui_robograph.h"

#include "../model/mrggraphmodel.h"

#include "../widget/lineentity.h"
#include "../widget/rectentity.h"
#include "../widget/imageentity.h"

#include "../../mrg/entityfactory.h"

EntityWidget* RoboGraph::createEntityWidget( EntityWidget::EntityWidgetType tpe,
                                             RoboGraph *parent,
                                             const QPoint &pt )
{
    Q_ASSERT( NULL != parent );

    EntityWidget *pWig;
    if ( tpe == EntityWidget::entity_widget_line )
    {
        pWig = new LineEntity( parent );
        if ( NULL == pWig )
        { return pWig; }

        pWig->setVisible( true );
        pWig->move( pt );

        connect( pWig, SIGNAL(signal_link_changed(EntityWidget*,Anchor::anchorType,QRect)),
                 parent, SLOT(slot_link_changed(EntityWidget*,Anchor::anchorType,QRect)));

        connect( pWig, SIGNAL(signal_request_delete(EntityWidget*)),
                 parent, SLOT(slot_request_delete(EntityWidget*)) );

        return pWig;
    }
    else if ( tpe == EntityWidget::entity_widget_rect )
    {
        pWig = new RectEntity(parent);
    }
    else if ( tpe == EntityWidget::entity_widget_image )
    {
        pWig = new ImageEntity(parent);
    }
    else
    { return NULL; }

    //! invalid
    if ( NULL != pWig )
    {}
    else
    { return NULL; }

    //! post proc
    pWig->setVisible( true );
    pWig->move( pt );

    connect( pWig, SIGNAL(signal_anchor_changed(EntityWidget*,Anchor::anchorType,QPoint)),
             parent, SLOT(slot_anchor_changed(EntityWidget*,Anchor::anchorType,QPoint)));

    connect( pWig, SIGNAL(signal_request_delete(EntityWidget*)),
             parent, SLOT(slot_request_delete(EntityWidget*)) );

//    mChildWidgets.append( pWig );

    //! \todo to model
//    parent

    return pWig;
}

RoboGraph::RoboGraph(QWidget *parent) :
    modelView(parent),
    ui(new Ui::RoboGraph)
{
    ui->setupUi(this);

    mFilePattern<<"Graph file"<<"mrg";
//    //! debug
//    m_pLineWidget = new LineEntity( this );
//    m_pLineWidget->setVisible( true );
//    connect( m_pLineWidget, SIGNAL(signal_link_changed(EntityWidget*,Anchor::anchorType,QRect)),
//             this, SLOT(slot_link_changed(EntityWidget*,Anchor::anchorType,QRect)));

//    m_pRectWidget = new RectEntity(this);
//    m_pRectWidget->setVisible( true );
//    connect( m_pRectWidget, SIGNAL(signal_anchor_changed(EntityWidget*,Anchor::anchorType,QPoint)),
//             this, SLOT(slot_anchor_changed(EntityWidget*,Anchor::anchorType,QPoint)));

//    m_pRectWidget->setGeometry( 200,200,100,50);

//    mChildWidgets.append( m_pLineWidget );
//    mChildWidgets.append( m_pRectWidget );

//    EntityWidget *pWig;
//    pWig = new ImageEntity( this );
//    pWig->setGeometry( 300,10,100,100 );

//    connect( pWig, SIGNAL(signal_anchor_changed(EntityWidget*,Anchor::anchorType,QPoint)),
//             this, SLOT(slot_anchor_changed(EntityWidget*,Anchor::anchorType,QPoint)));

//    mChildWidgets.append( pWig );
//    setMouseTracking( true );
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
    QPoint ptChild;
    for ( int i = 0; i < ((MrgGraphModel *)m_pModelObj)->mChildWidgets.size(); i++ )
    {
        ptChild = ((MrgGraphModel *)m_pModelObj)->mChildWidgets.at(i)->mapFromParent( mPtMouse );

        if ( ((MrgGraphModel *)m_pModelObj)->mChildWidgets.at(i)->mouseOver(ptChild) )
        { ((MrgGraphModel *)m_pModelObj)->mChildWidgets[i]->setSelected( true ); }
        else
        { ((MrgGraphModel *)m_pModelObj)->mChildWidgets[i]->setSelected( false ); }
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
    ShiftContext context;
    for ( int i = 0; i < ((MrgGraphModel *)m_pModelObj)->mChildWidgets.size(); i++ )
    {
        if ( ((MrgGraphModel *)m_pModelObj)->mChildWidgets.at(i)->selected() )
        {
            wigRect = ((MrgGraphModel *)m_pModelObj)->mChildWidgets.at(i)->geometry();

            ((MrgGraphModel *)m_pModelObj)->mChildWidgets.at(i)->shift(  wigRect, QPoint( dx, dy ), context );
            ((MrgGraphModel *)m_pModelObj)->mChildWidgets.at(i)->setGeometry( wigRect );
            ((MrgGraphModel *)m_pModelObj)->mChildWidgets.at(i)->shiftEnd( context );

            //! link
            ((MrgGraphModel *)m_pModelObj)->mChildWidgets.at(i)->anchorProc();
        }
    }

    //! save now
    mPtMouse = ptNow;

    //! restore cursor

    modelView::mouseMoveEvent( event );
}

void RoboGraph::mouseDoubleClickEvent( QMouseEvent *event )
{
    if ( event->buttons() == Qt::LeftButton )
    {
        EntityWidget *pWig = createEntityWidget( EntityWidget::entity_widget_line, this, event->pos() );
        if ( NULL != pWig )
        {
            addEntity( pWig );
        }
    }
    else
    { return; }

    //! add line
}

void RoboGraph::keyReleaseEvent(QKeyEvent *event)
{
    if( event->key() == Qt::Key_Delete )
    {logDbg();
        deleteSelected();
    }
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
                                             event->mimeData()->text(),
                                             pt );
        if ( NULL == pWig )
        { return; }
    }
    //! operator
    else if ( event->mimeData()->hasFormat("oplib/operator") )
    {
        event->acceptProposedAction();

        EntityWidget *pWig = addOperationEntity( event->mimeData()->text(),
                                                event->mimeData()->data( "oplib/operator"),
                                                pt );

        if ( NULL == pWig )
        { return; }
    }
    else
    {

    }
}

//QSize RoboGraph::sizeHint()
//{
//    if ( mChildWidgets.size() > 0 )
//    {
//        QPolygon poly;

//        for( int i = 0; i < mChildWidgets.size(); i++ )
//        {
//            poly<<QPolygon( mChildWidgets[i]->geometry() );
//        }

//        return poly.boundingRect().size();
//    }
//    else
//    { return modelView::sizeHint(); }
//}

int RoboGraph::save( QString &outFileName )
{
    //! get obj
    MrgGraphModel *pModel = dynamic_cast< MrgGraphModel*>( m_pModelObj );
    if ( NULL == pModel )
    { logDbg(); return -1; }

    //! build name
    outFileName = m_pModelObj->getPath() + QDir::separator() + m_pModelObj->getName();
    outFileName = QDir::toNativeSeparators( outFileName );

    QFile file(outFileName);
    if ( file.open( QIODevice::WriteOnly ) )
    {}
    else
    { return -1; }

    return pModel->save( outFileName );
}
int RoboGraph::saveAs( QString &name )
{
    MrgGraphModel *pModel = dynamic_cast< MrgGraphModel*>( m_pModelObj );
    if ( NULL == pModel )
    { return -1; }

    return pModel->save( name );
}

int RoboGraph::load( const QString &name, const QString &path )
{
    //! get obj
    MrgGraphModel *pModel = dynamic_cast< MrgGraphModel*>( m_pModelObj );
    if ( NULL == pModel )
    { logDbg(); return -1; }

    //! clean
    pModel->clean();

    int ret = pModel->load( name, this );

    return ret;
}

//! rect in parent
void RoboGraph::slot_link_changed( EntityWidget *pWig,
                                   Anchor::anchorType tpe,
                                   QRect rect )
{
    Q_ASSERT( NULL != pWig );

    //! detect success
    EntityWidget *pAttachWig;

    if ( crossDetect( pWig, rect, &pAttachWig ) )
    {logDbg();}
    else
    {logDbg();
        pWig->delink( tpe );
        return;
    }

    //! stick to border
    QRect geo;
    geo = pAttachWig->geometry();

    QPoint pt = rect.center();

    QPoint ptStick;
    ptStick.setX( qMin( geo.right(), qMax( geo.left(), pt.x() ) ) );
    ptStick.setY( qMin( geo.bottom(), qMax( geo.top(), pt.y() ) ) );

    //! in range now
    int dists[4];

    dists[0] = qAbs ( ptStick.y() - geo.top() );
    dists[1] = qAbs ( ptStick.x() - ( geo.left() + geo.width() ) );
    dists[2] = qAbs ( ptStick.y() - ( geo.top() + geo.height() ) );
    dists[3] = qAbs ( ptStick.x() - geo.left() );

    //! select the min
    int distMin;
    distMin = dists[0];
    for ( int i = 1; i < 4; i++ )
    { distMin = qMin( distMin, dists[i]); }

    //! now stick
    if ( distMin == dists[0] )
    { ptStick.setY( geo.top() ); }
    else if ( distMin == dists[1] )
    { ptStick.setX( geo.left() + geo.width() ); }
    else if ( distMin == dists[2] )
    { ptStick.setY( geo.top() + geo.height() ); }
    else if ( distMin == dists[3] )
    { ptStick.setX( geo.left() ); }
    else
    {}

    //! \todo stick overlap

    //! pt stick ref to the widget
    pAttachWig->attachWidget( pWig, tpe, ptStick - geo.topLeft() );
}

void RoboGraph::slot_anchor_changed( EntityWidget *pWig,
                          Anchor::anchorType tpe,
                          QPoint ptParent )
{
    Q_ASSERT( NULL != pWig );

    QPoint ptRef;
    QRect rect;
    Anchor::anchorType refType;
    if ( tpe == Anchor::anchor_line_from )
    {logDbg();
        ptRef = pWig->toAnchor();
        refType = Anchor::anchor_line_to;
    }
    else if ( tpe == Anchor::anchor_line_to )
    {
        ptRef = pWig->fromAnchor();
        refType = Anchor::anchor_line_from;
    }
    else
    { return; }

    //! gen
    rect = EntityWidget::genBoundingRect( ptParent, ptRef );
logDbg()<<rect<<ptParent<<ptRef;
    //! geo
    pWig->setGeometry( rect );
    pWig->setAnchor( tpe, pWig->mapFromParent( ptParent ) );
    pWig->setAnchor( refType, pWig->mapFromParent( ptRef ) );
}

void RoboGraph::slot_request_delete( EntityWidget *pWig )
{
    deleteSelected();
}

void RoboGraph::deleteSelected()
{
    QList<EntityWidget*> delList;

    //! collect
    foreach (EntityWidget *pWig, ((MrgGraphModel *)m_pModelObj)->mChildWidgets )
    {
        if ( pWig->selected() )
        { delList.append( pWig ); }
    }
logDbg();
    //! delete
    foreach( EntityWidget *pWig, delList )
    {logDbg();
        //! \note link and attach are conflict
        pWig->cleanAttach();

        pWig->cleanLink();

        ((MrgGraphModel *)m_pModelObj)->mChildWidgets.removeAll( pWig );

        delete pWig;
    }
}

void RoboGraph::addEntity( EntityWidget *pEntity )
{
    Q_ASSERT( NULL != pEntity );

    Q_ASSERT( NULL != m_pModelObj );
    ((MrgGraphModel *)m_pModelObj)->mChildWidgets.append( pEntity );
}

EntityWidget* RoboGraph::addRoboEntity( const QString &className,
                                        const QString &name,
                                        const QPoint &pt )
{
    //! create a new robot
    VRobot *pNewRobot = robotFact::createRobot( className );
    Q_ASSERT( NULL != pNewRobot );

    ImageEntity *pWig = (ImageEntity *)createEntityWidget( EntityWidget::entity_widget_image, this, pt );
    if ( NULL != pWig )
    {
        //! \todo for the model

        addEntity( pWig );

        pWig->setLabel( name );
        pWig->setDataClass( className );
    }
    else
    { }

    delete pNewRobot;

    return pWig;
}

EntityWidget* RoboGraph::addOperationEntity( const QString &className,
                                            const QString &icon,
                                            const QPoint &pt )
{
    ImageEntity *pWig = (ImageEntity *)createEntityWidget( EntityWidget::entity_widget_image, this, pt );
    if ( NULL != pWig )
    {
        //! model attached
        Entity *pModel = EntityFactory::createEntity( className );
        if ( NULL == pModel )
        {
            delete pWig;
            return NULL;
        }
        else
        {}

        pWig->setModel( pModel );

        //! entity
        addEntity( pWig );

        pWig->setLabel( className );
        pWig->setImage( ( icon ) );
    }
    else
    { }

    return pWig;
}

bool RoboGraph::crossDetect( EntityWidget *pSrc,
                             const QRect &rect,
                             EntityWidget **pWig )
{
    QRect geo;
    for ( int i = 0; i < ((MrgGraphModel *)m_pModelObj)->mChildWidgets.size(); i++ )
    {
        if ( pSrc != ((MrgGraphModel *)m_pModelObj)->mChildWidgets.at(i) )
        {}
        else
        { continue; }

        geo = ((MrgGraphModel *)m_pModelObj)->mChildWidgets.at(i)->geometry();
        logDbg()<<geo<<rect;
        if ( geo.intersects( rect ) )
        {
            *pWig = ((MrgGraphModel *)m_pModelObj)->mChildWidgets.at(i);
            return true;
        }
    }

    return false;
}

void RoboGraph::on_btnGen_clicked()
{
    ((MrgGraphModel *)m_pModelObj)->mChildWidgets.at(0)->iterAttached();
}

void RoboGraph::on_btnRun_clicked()
{

}

void RoboGraph::on_btnStop_clicked()
{

}
