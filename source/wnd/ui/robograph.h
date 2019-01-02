#ifndef ROBOGRAPH_H
#define ROBOGRAPH_H

#include <QWidget>

#include "modelview.h"

#include "../widget/entitywidget.h"

namespace Ui {
class RoboGraph;
}

class RoboGraph : public modelView
{
    Q_OBJECT

public:
    //! entity op
    static EntityWidget* createEntityWidget( EntityWidget::EntityWidgetType tpe,
                                      RoboGraph *parent,
                                      const QPoint &pt=QPoint(0,0) );

public:
    explicit RoboGraph(QWidget *parent = 0);
    ~RoboGraph();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent( QMouseEvent *event );
    virtual void mouseMoveEvent(QMouseEvent *event);

    virtual void mouseDoubleClickEvent( QMouseEvent *event );

    virtual void keyReleaseEvent(QKeyEvent *event);

    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);

//    virtual QSize sizeHint();
public:
    virtual int save( QString &outFileName );
    virtual int saveAs( QString &name );
    virtual int load( const QString &name, const QString &path );

protected slots:
    void slot_link_changed( EntityWidget *pWig, Anchor::anchorType tpe, QRect rect );
    void slot_anchor_changed( EntityWidget *pWig,
                              Anchor::anchorType tpe,
                              QPoint ptParent );
    void slot_request_delete( EntityWidget *pWig );

protected:
    void selectAll();
    void deSelectAll();
    void deleteSelected();

    void addEntity( EntityWidget *pEntity );

    EntityWidget* addRoboEntity( const QString &className,
                                 const QString &name,
                                 const QPoint &pt );
    EntityWidget* addOperationEntity( const QString &className,
                                     const QString &name,
                                     const QPoint &pt );

    bool crossDetect( EntityWidget *pSrc,
                      const QRect &rect,
                      EntityWidget **pWig );

    //! diagram arith
    bool diagramCompile();

    bool diagramCheck();
    bool diagramGen();

private slots:
    void on_btnGen_clicked();

    void on_btnRun_clicked();

    void on_btnStop_clicked();

private:
    Ui::RoboGraph *ui;

//    EntityWidget *m_pLineWidget;
//    EntityWidget *m_pRectWidget;

//    QList<EntityWidget*> mChildWidgets;

    QPoint mPtMouse;
};

#endif // ROBOGRAPH_H
