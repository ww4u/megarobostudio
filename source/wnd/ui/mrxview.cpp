#include "mrxview.h"

mrxView::mrxView( QWidget *parent ) : modelView( parent )
{
    mAxesId = 0;
}

void mrxView::setAxesId( int id )
{ mAxesId = id; }
int mrxView::getAxesId()
{ return mAxesId; }

void mrxView::modelChanged()
{}
