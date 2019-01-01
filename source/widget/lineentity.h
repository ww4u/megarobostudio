#ifndef LINEENTITY_H
#define LINEENTITY_H

#include "entitywidget.h"
#include <QMouseEvent>
class LineEntity : public EntityWidget
{
    Q_OBJECT

public:
    enum linePos
    {
        line_unk,
        line_from,
        line_to,
        line_center,
    };

public:
    LineEntity( QWidget *parent = NULL );

protected:
    virtual void paintEvent( QPaintEvent *event );

protected:
    //! ref to self
    virtual void mousePressEvent( QMouseEvent *event );
    virtual void mouseReleaseEvent( QMouseEvent *event );
    virtual void mouseMoveEvent( QMouseEvent *event );

public:
    virtual int serialOut( QXmlStreamWriter &writer, QList<EntityWidget *> &refWidgets );
    virtual int serialIn( QXmlStreamReader &reader );

protected:
    void fromChanged();
    void toChanged();

public:
    virtual bool mouseOver( const QPoint &pt);
    virtual void shift( QRect &geometry,
                        const QPoint &pt,
                        ShiftContext &context );
    virtual void shiftEnd( ShiftContext &context );

    virtual void setAnchor(Anchor::anchorType typ, QPoint val);

    //! anchor in parent
    virtual QPoint fromAnchor();
    virtual QPoint toAnchor();

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

    linePos mPos;

};

#endif // LINEENTITY_H
