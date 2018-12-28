#include "entitywidget.h"

#include <QPainter>
#include <QKeyEvent>
#include "../../include/mydebug.h"

int EntityWidget::_focus_radius = 8;

QRect EntityWidget::genBoundingRect( QPoint a, QPoint b )
{
    QRect ra, rb;

    ra.setTopLeft( a - QPoint( EntityWidget::_focus_radius, EntityWidget::_focus_radius) );
    ra.setSize( QSize( EntityWidget::_focus_radius * 2, EntityWidget::_focus_radius * 2 ) );

    rb.setTopLeft( b - QPoint( EntityWidget::_focus_radius, EntityWidget::_focus_radius) );
    rb.setSize( QSize( EntityWidget::_focus_radius * 2, EntityWidget::_focus_radius * 2 ) );

    QPolygon poly;

    poly<<QPolygon(ra)<<QPolygon(rb);

    return poly.boundingRect();
}

EntityWidget::EntityWidget(QWidget *parent) : QFrame(parent)
{
    m_pModel = NULL;

    mWidth = 1;
    mColor = Qt::gray;
    mStyle = Qt::SolidLine;

    mbSelected = false;
    mbPressed = false;

    setMouseTracking( true );
}

void EntityWidget::paintEvent( QPaintEvent *event )
{
    QFrame::paintEvent( event );

    QPainter painter( this );
    paintFrame( painter );
}

void EntityWidget::mousePressEvent( QMouseEvent *event )
{
    mbPressed = true;
    QFrame::mousePressEvent( event );
}

void EntityWidget::mouseReleaseEvent( QMouseEvent *event )
{
    mbPressed = false;

    setCursor( Qt::ArrowCursor );

    QFrame::mouseReleaseEvent( event );

//    logDbg();
}

//void EntityWidget::keyReleaseEvent(QKeyEvent *event)
//{
//    QFrame::keyReleaseEvent( event );
//logDbg();
//    if ( event->key() == Qt::Key_Delete )
//    {logDbg();
//        emit signal_request_delete( this );
//    }
//}

void EntityWidget::paintFrame( QPainter &painter )
{
    if ( mbSelected )
    {
        painter.save();

        QPen tPen;
        tPen.setColor( Qt::blue );
        tPen.setStyle( Qt::SolidLine );
        tPen.setWidth( 1 );

        painter.setPen( tPen );

        QRect tRect;
        tRect = rect();
        tRect = tRect.adjusted( 0,0,-1,-1 );
        painter.drawRect( tRect );

        painter.restore();
    }
}

QRect EntityWidget::genFocusRect( const QPoint &pt )
{
    QPoint ptLt;

    ptLt = pt - QPoint( _focus_radius, _focus_radius );

    return QRect( ptLt, QSize( _focus_radius * 2, _focus_radius*2) );
}

//! point in rect
bool EntityWidget::mouseOver( const QPoint &pt )
{
    return rect().contains( pt );
}

void EntityWidget::shift( QRect &geometry,
                          const QPoint &pt,
                          ShiftContext &context )
{
    geometry.adjust( pt.x(), pt.y(), pt.x(), pt.y() );
}

void EntityWidget::shiftEnd( ShiftContext &context )
{}

void EntityWidget::setAnchor( Anchor::anchorType typ, QPoint val )
{}

QPoint EntityWidget::fromAnchor()
{ return QPoint(); }
QPoint EntityWidget::toAnchor()
{ return QPoint(); }

void EntityWidget::anchorProc()
{
    QMapIterator<EntityWidget *, Anchor > iter( mAttachedWidgets );
    QPoint pt;
    while( iter.hasNext() )
    {
        iter.next();

        pt.setX( iter.value().mRefPt.x() * rect().width() );
        pt.setY( iter.value().mRefPt.y() * rect().height() );

        pt = mapToParent( pt );
        logDbg()<<pt;

        emit signal_anchor_changed( iter.key(), iter.value().mType, pt );
    }
}

void EntityWidget::attachEntity( Entity *p )
{
    Q_ASSERT( NULL != p );

    m_pModel = p;
}
Entity *EntityWidget::entity()
{ return m_pModel; }

void EntityWidget::setColor( QColor color )
{ mColor = color; }
QColor EntityWidget::color()
{ return mColor; }

void EntityWidget::setWidth( int wid )
{
    if ( wid > 0 )
    { mWidth = wid; }
    //! do nothing
    else
    {  }
}
int EntityWidget::width()
{ return mWidth; }

void EntityWidget::setStyle( Qt::PenStyle style )
{ mStyle = style; }
Qt::PenStyle EntityWidget::style()
{ return mStyle; }

void EntityWidget::setSelected( bool b )
{
    mbSelected = b;
    if ( mbSelected )
    {
        setFocus();
        raise();
    } //! to top
    update();
}
bool EntityWidget::selected()
{ return mbSelected; }

//! pt ref to the widget
void EntityWidget::attachWidget( EntityWidget *pWig,
                                 Anchor::anchorType arg,
                                 const QPoint &pt )
{
    Q_ASSERT( NULL != pWig );

    //! detach from the link
    pWig->delink( arg );

    //! attached

    //! build anchor
    Anchor anc;
    anc.mType = arg;
    anc.mRefPt.setX( (qreal)pt.x()/rect().width() );
    anc.mRefPt.setY( (qreal)pt.y()/rect().height() );
logDbg()<<pt<<rect()<<anc.mRefPt;
    //! insert arg
    mAttachedWidgets.insert( pWig, anc );

    pWig->link( arg, this );
}
void EntityWidget::detachWidget( EntityWidget *pWig )
{logDbg();
    mAttachedWidgets.remove( pWig );
}

void EntityWidget::cleanAttach()
{
    QMapIterator<EntityWidget *, Anchor > iter( mAttachedWidgets );
    while( iter.hasNext() )
    {
        iter.next();
        iter.key()->unlink( iter.value().mType );
    }
}

void EntityWidget::link( Anchor::anchorType arg, EntityWidget *pWig )
{
    mLinkWidget.insert( arg, pWig );
}
void EntityWidget::unlink( Anchor::anchorType arg )
{
    mLinkWidget.remove( arg );
}
void EntityWidget::delink( Anchor::anchorType arg )
{
    if ( mLinkWidget.contains( arg ) )
    {logDbg();
        mLinkWidget[arg]->detachWidget( this );
        mLinkWidget.remove( arg );
    }
    else
    { logDbg(); }
}

void EntityWidget::cleanLink()
{
    QMapIterator< Anchor::anchorType, EntityWidget *>iter( mLinkWidget );
    while( iter.hasNext() )
    {
        iter.next();
        iter.value()->detachWidget( this );
    }
}

void EntityWidget::setPenAttr( QPen &pen )
{
    pen.setWidth( mWidth );
    pen.setColor( mColor );
    pen.setStyle( mStyle );
}
