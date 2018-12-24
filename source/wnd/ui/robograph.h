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
    explicit RoboGraph(QWidget *parent = 0);
    ~RoboGraph();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent( QMouseEvent *event );
    virtual void mouseMoveEvent(QMouseEvent *event);

    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);
public:
    virtual int save( QString &outFileName );
    virtual int saveAs( QString &name );

protected:
    EntityWidget* addRoboEntity( const QString &className, const QString &name );
    EntityWidget* addOperatorEntity( const QString &className, const QString &name );

private:
    Ui::RoboGraph *ui;

    EntityWidget *m_pLineWidget;
    EntityWidget *m_pRectWidget;

    QList<EntityWidget*> mChildWidgets;

    QPoint mPtMouse;
};

#endif // ROBOGRAPH_H
