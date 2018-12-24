#include "entitywidget.h"

#include <QPainter>
#include "../../include/mydebug.h"
EntityWidget::EntityWidget(QWidget *parent) : QFrame(parent)
{
    m_pModel = NULL;

    mWidth = 1;
    mColor = Qt::gray;
    mStyle = Qt::SolidLine;

    mbSelected = false;
}

void EntityWidget::paintEvent( QPaintEvent *event )
{
    QFrame::paintEvent( event );

    QPainter painter( this );
    paintFrame( painter );
}

void EntityWidget::paintFrame( QPainter &painter )
{
    if ( mbSelected )
    {
        painter.save();

        QPen tPen;
        tPen.setColor( Qt::blue );
        tPen.setStyle( Qt::DotLine );
        tPen.setWidth( 1 );

        painter.setPen( tPen );

        QRect tRect;
        tRect = rect();
        tRect = tRect.adjusted( 0,0,-1,-1 );
        painter.drawRect( tRect );

        painter.restore();
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
    update();
}
bool EntityWidget::selected()
{ return mbSelected; }

void EntityWidget::setPenAttr( QPen &pen )
{
    pen.setWidth( mWidth );
    pen.setColor( mColor );
    pen.setStyle( mStyle );
}
