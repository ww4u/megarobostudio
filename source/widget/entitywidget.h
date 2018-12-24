#ifndef ENTITYWIDGET_H
#define ENTITYWIDGET_H

#include <QFrame>
#include <QPainter>

class Entity;

class EntityWidget : public QFrame
{
    Q_OBJECT
public:
    explicit EntityWidget(QWidget *parent = nullptr);    

signals:

public slots:

protected:
    virtual void paintEvent( QPaintEvent *event );

protected:
    void paintFrame( QPainter &painter );

public:
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
};

#endif // ENTITYWIDGET_H
