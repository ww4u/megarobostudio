#include "lineentity.h"
#include <QPainter>
LineEntity::LineEntity( QWidget *parent ) : EntityWidget( parent )
{
    mFrom.setX( 0 );
    mFrom.setY( 0 );

    mTo.setX( 99 );
    mTo.setY( 199 );

    setGeometry( 10,10, 100,200 );

    setFrameShape( QFrame::NoFrame );
}

void LineEntity::paintEvent( QPaintEvent *event )
{
    QPainter painter( this );

    //! frame
    paintFrame( painter );

    QPen pen;
    setPenAttr( pen );
    painter.setPen( pen );

    painter.drawLine( mFrom, mTo );

    //! draw the arrow
    //! \todo
}

void LineEntity::setFrom( QPoint pt )
{ mFrom = pt; }
QPoint LineEntity::from()
{ return mFrom; }

void LineEntity::setTo( QPoint pt )
{ mTo = pt; }
QPoint LineEntity::to()
{ return mTo; }


