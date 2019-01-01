#include "lineentity.h"
#include <QPainter>

LineEntity::LineEntity( QWidget *parent ) : EntityWidget( parent )
{
    mEntityType = EntityWidget::entity_widget_line;

    mFrom.setX( _focus_radius );
    mFrom.setY( _focus_radius );

    mTo.setX( 99 - _focus_radius );
    mTo.setY( _focus_radius );

    mPos = line_unk;

    setGeometry( 0,0, 100,_focus_radius * 2 );

    //! no style
    setStyleSheet( "QFrame "
    "{"
        "border: 0px;"
        "background: transparent;"
    "}"
                   );
}

void LineEntity::paintEvent( QPaintEvent *event )
{
    QPainter painter( this );

    //! frame
//    paintFrame( painter );

    QPen pen;
    setPenAttr( pen );
    painter.setPen( pen );

    painter.drawLine( mFrom, mTo );

    //! draw the arrow
    //! \todo
    QPainterPath path;
    path.addEllipse( mTo, _focus_radius/2, _focus_radius/2 );
    painter.fillPath( path, mColor );

    if ( mbSelected )
    {
        QPen tPen;
        tPen.setColor( Qt::blue );
        tPen.setStyle( Qt::SolidLine );
        tPen.setWidth( 1 );

        painter.setPen( tPen );

        painter.drawEllipse( mFrom, _focus_radius, _focus_radius );
        painter.drawEllipse( mTo, _focus_radius, _focus_radius );

        //! center
        QPoint ptCenter;
        ptCenter = (mFrom + mTo)/2;
        QRect rectCenter = genFocusRect( ptCenter );
        painter.drawRect( rectCenter );
    }
}

void LineEntity::mousePressEvent( QMouseEvent *event )
{
    //! point
    QPoint pt = event->pos();

    QPoint ptCenter = ( mFrom + mTo ) / 2;

    //! focus rect
    QRect rectLt, rectRd, rectCenter;

    rectLt = genFocusRect( mFrom );
    rectRd = genFocusRect( mTo );
    rectCenter = genFocusRect( ptCenter );

    //! check
    if ( rectLt.contains( pt ) )
    {
        setCursor( Qt::PointingHandCursor );
        mPos = line_from;
    }
    else if( rectRd.contains( pt ) )
    {
        setCursor( Qt::PointingHandCursor );
        mPos = line_to;
    }
    else if ( rectCenter.contains( pt ) )
    {
        setCursor( Qt::ClosedHandCursor );
        mPos = line_center;
    }
    else
    {
        setCursor( Qt::ArrowCursor );
        mPos = line_center;
    }

    EntityWidget::mousePressEvent( event );
}

void LineEntity::mouseReleaseEvent( QMouseEvent *event )
{
    EntityWidget::mouseReleaseEvent( event );

    //! only valid for link changed
    if ( mPos == line_from )
    {
        fromChanged();
    }
    else if( mPos == line_to )
    {
        toChanged();
    }
    else
    {
//        logDbg();
        if ( mbSelected )
        {
            fromChanged();
            toChanged();
        }
    }
}

int LineEntity::serialOut( QXmlStreamWriter &writer, QList<EntityWidget *> &refWidgets )
{
    int ret = 0;
    writer.writeStartElement( "base" );
    ret = EntityWidget::serialOut( writer, refWidgets );
    writer.writeEndElement();

    writer.writeStartElement( "sub" );

        writer.writeTextElement( "from", fmtPoint( mFrom ));
        writer.writeTextElement( "to", fmtPoint( mTo ));

    writer.writeEndElement();

    return ret;
}
int LineEntity::serialIn( QXmlStreamReader &reader )
{
    int ret = 0;
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "base" )
        {
            ret = EntityWidget::serialIn( reader );
        }
        else if ( reader.name() == "sub" )
        {
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "from" )
                { mFrom = toPoint( reader.readElementText()); }
                else if ( reader.name() == "to" )
                { mTo = toPoint( reader.readElementText()); }
                else
                { reader.skipCurrentElement(); }
            }
        }
        else
        { reader.skipCurrentElement(); }
    }

    return ret;
}

void  LineEntity::mouseMoveEvent( QMouseEvent *event )
{
    //! point
    QPoint pt = event->pos();

    QPoint ptCenter = ( mFrom + mTo ) / 2;

    //! focus rect
    QRect rectLt, rectRd, rectCenter;

    rectLt = genFocusRect( mFrom );
    rectRd = genFocusRect( mTo );
    rectCenter = genFocusRect( ptCenter );

    //! check
    if ( rectLt.contains( pt ) )
    {
        if ( ( Qt::LeftButton  & event->buttons() ) == Qt::LeftButton )
        { }
        else
        { setCursor( Qt::PointingHandCursor ); }
    }
    else if( rectRd.contains( pt ) )
    {
        if ( ( Qt::LeftButton  & event->buttons() ) == Qt::LeftButton )
        { }
        else
        { setCursor( Qt::PointingHandCursor ); }
    }
    else if ( rectCenter.contains( pt ) )
    {
        if ( ( Qt::LeftButton  & event->buttons() ) == Qt::LeftButton )
        { }
        else
        { setCursor( Qt::OpenHandCursor ); }
    }
    else
    {
        if ( ( Qt::LeftButton  & event->buttons() ) == Qt::LeftButton )
        { }
        else
        { setCursor( Qt::ArrowCursor ); }
    }

    EntityWidget::mouseMoveEvent( event );
}

void LineEntity::fromChanged()
{
    QPoint pt;
    QRect rect;

    pt = mapToParent( mFrom );

    rect.setTopLeft( pt - QPoint( _focus_radius, _focus_radius ) );
    rect.setSize( QSize( 2*_focus_radius, 2*_focus_radius ) );

    emit signal_link_changed( this, Anchor::anchor_line_from, rect );
}
void LineEntity::toChanged()
{
    QPoint pt;
    QRect rect;

    pt = mapToParent( mTo );

    rect.setTopLeft( pt - QPoint( _focus_radius, _focus_radius ) );
    rect.setSize( QSize( 2*_focus_radius, 2*_focus_radius ) );

    emit signal_link_changed( this, Anchor::anchor_line_to, rect );
}

//! pt in global
bool LineEntity::mouseOver( const QPoint &pt)
{
    QPainterPathStroker stroker;
    stroker.setWidth( _focus_radius );

    QPainterPath path;
    path.moveTo( mFrom );
    path.lineTo( mTo );

    QPainterPath tPath;
    tPath = stroker.createStroke( path );

    return tPath.contains( pt );
}

void LineEntity::shift( QRect &geometry,
                        const QPoint &pt,
                        ShiftContext &context )
{
    QPoint ptFrom, ptTo;
    if ( mPos == line_from )
    {
        ptFrom = mapToParent( mFrom );
        ptTo = mapToParent( mTo );

        ptFrom += pt;

        context.v1 = ptFrom;
        context.v2 = ptTo;
    }
    else if ( mPos == line_to )
    {
        ptFrom = mapToParent( mFrom );
        ptTo = mapToParent( mTo );

        ptTo += pt;

        context.v1 = ptFrom;
        context.v2 = ptTo;
    }
    else if ( mPos == line_center )
    { return EntityWidget::shift( geometry, pt, context ); }
    else
    { return; }

    //! change geo
    QRect rectFrom, rectTo;
    rectFrom.setTopLeft( ptFrom - QPoint( _focus_radius, _focus_radius ) );
    rectFrom.setSize( QSize( _focus_radius*2, _focus_radius*2) );

    rectTo.setTopLeft( ptTo - QPoint( _focus_radius, _focus_radius ) );
    rectTo.setSize( QSize( _focus_radius*2, _focus_radius*2) );

    QPolygon poly;
    poly<<QPolygon(rectFrom)<<QPolygon(rectTo);

    geometry = poly.boundingRect();
}

void LineEntity::shiftEnd( ShiftContext &context )
{
    if ( mPos == line_from || mPos==line_to  )
    {
        mFrom = mapFromParent( context.v1 );
        mTo = mapFromParent( context.v2 );
    }
    else
    {}
}

void LineEntity::setAnchor(Anchor::anchorType tpe, QPoint val)
{
    if ( tpe == Anchor::anchor_line_from )
    { mFrom = val; }
    else if ( tpe == Anchor::anchor_line_to )
    { mTo = val; }
    else
    {}
}

QPoint LineEntity::fromAnchor()
{
    return mapToParent( mFrom );
}
QPoint LineEntity::toAnchor()
{
    return mapToParent( mTo );
}

void LineEntity::setFrom( QPoint pt )
{ mFrom = pt; }
QPoint LineEntity::from()
{ return mFrom; }

void LineEntity::setTo( QPoint pt )
{ mTo = pt; }
QPoint LineEntity::to()
{ return mTo; }


