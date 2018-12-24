#ifndef LINEENTITY_H
#define LINEENTITY_H

#include "entitywidget.h"

class LineEntity : public EntityWidget
{
    Q_OBJECT

public:
    LineEntity( QWidget *parent = NULL );

protected:
    virtual void paintEvent( QPaintEvent *event );

public:
    void setFrom( QPoint pt );
    QPoint from();

    void setTo( QPoint pt );
    QPoint to();

protected:
    QPoint mFrom, mTo;

    QColor mColor;
    int mWidth;
    Qt::PenStyle mStyle;

};

#endif // LINEENTITY_H
