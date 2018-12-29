#ifndef ENTITYWIDGET_H
#define ENTITYWIDGET_H

#include <QFrame>
#include <QPainter>
#include <QMap>
#include <QtCore>
#include "../../include/mydebug.h"

class Entity;

class ShiftContext
{
public:
    QPoint v1, v2;
};

class Anchor
{
public:
    enum anchorType
    {
        anchor_unk,
        anchor_line_from,
        anchor_line_to
    };

public:
    anchorType mType;
    QPointF mRefPt;
};

class EntityWidget : public QFrame
{
    Q_OBJECT

public:
    enum EntityWidgetType
    {
        entity_widget_unk,
        entity_widget_line,
        entity_widget_rect,
        entity_widget_image,
    };

public:
    static int _focus_radius;

public:
    static QRect genBoundingRect( QPoint a, QPoint b );

public:
    explicit EntityWidget(QWidget *parent = nullptr);    

signals:
    void signal_link_changed( EntityWidget *pWig,
                              Anchor::anchorType tpe,
                              QRect hotRect );

    void signal_anchor_changed( EntityWidget *pWig,
                                Anchor::anchorType tpe,
                                QPoint ptParent );

    void signal_request_delete( EntityWidget *pWig );

public slots:

protected:
    virtual void paintEvent( QPaintEvent *event );

    virtual void mousePressEvent( QMouseEvent *event );
    virtual void mouseReleaseEvent( QMouseEvent *event );

//    virtual void keyReleaseEvent(QKeyEvent *event);

protected:
    QString fmtRect( const QRect &rect );
    QRect toRect( const QString &str );

    QString fmtPointF( const QPointF &pt );
    QPointF toPointF( const QString &str );

public:
    int serialOut( QXmlStreamWriter &writer, QList<EntityWidget *> &refWidgets );
    int serialIn( QXmlStreamReader &reader );

    void crossLink( const QList<EntityWidget *> &refWidgets );

protected:
    void paintFrame( QPainter &painter );

    QRect genFocusRect( const QPoint &pt );

public:
    virtual bool mouseOver( const QPoint &pt );
    virtual void shift( QRect &geometry,
                        const QPoint &pt,
                        ShiftContext &context );
    virtual void shiftEnd( ShiftContext &context );

    virtual void setAnchor( Anchor::anchorType typ, QPoint val );

    //! anchor in parent
    virtual QPoint fromAnchor();
    virtual QPoint toAnchor();

public:
    void anchorProc();

public:
    void setEntityType( EntityWidgetType tpe );
    EntityWidgetType entityType();

    void attachEntity( Entity *p );
    Entity *entity();

    void setColor( QColor color );
    QColor color();

    void setWidth( int wid );
    int width();

    void setStyle( Qt::PenStyle style );
    Qt::PenStyle style();

    void setSelected( bool b );
    bool selected();

    //! attach & detach widget
    void attachWidget( EntityWidget *pWig, Anchor::anchorType arg, const QPoint &pt );
    void detachWidget( EntityWidget *pWig );

    void cleanAttach();

    void link( Anchor::anchorType arg, EntityWidget *pWig );
    void unlink( Anchor::anchorType arg );
    void delink( Anchor::anchorType arg );

    void cleanLink();

protected:
    void setPenAttr( QPen &pen );

protected:
    Entity *m_pModel;

    //! pen
    QColor mColor;
    int mWidth;
    Qt::PenStyle mStyle;

    bool mbSelected;
    QPoint mPressPt;

    bool mbPressed;

    //! attr
    EntityWidgetType mEntityType;

    //! attached widget
    QMap<EntityWidget *, Anchor > mAttachedWidgets;
    QMap< Anchor::anchorType, EntityWidget *> mLinkWidget;

    //! local for serial in used
    QMap< int, Anchor > mRefAttached;
    QMap< Anchor::anchorType, int > mRefLink;
};

#endif // ENTITYWIDGET_H
