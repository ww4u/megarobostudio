#ifndef SCENEITEM_H
#define SCENEITEM_H

#include <QtWidgets>
#include "../../robot/robotfact.h"
#include "../../model/mcmodelobj.h"

namespace Ui {
class sceneWidget;
}


class sceneWidget : public QWidget
{
    Q_OBJECT
public:
    sceneWidget( QWidget *parent = 0 );
    ~sceneWidget();

protected:
    virtual void paintEvent( QPaintEvent *evt );

Q_SIGNALS:
    void editingFinished( sceneWidget *pWig, const QString &name );

protected Q_SLOTS:
    void slot_editingFinished();

public:
    void setRobot( VRobot *pBase );
    VRobot *getRobot();

    void setModelObj( mcModelObj *pBase );
    mcModelObj *getModelObj();

    bool matchWidget( QLabel *plabel );

    void setSelected( bool b );
    bool getSelected();

    void setName( const QString &name );
    QString getName();

    void focusOnName();

protected:
    VRobot *m_pBase;     //! robot
    mcModelObj *m_pModel;

    bool mbSelected;
    QString mName;

private:
    Ui::sceneWidget *ui;

};

#endif // SCENEITEM_H
