#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QtWidgets>

#include "../../robot/robotbase.h"
#include "../../model/mcmodelobj.h"

namespace Ui {
class sceneWidget;
}

class sceneWidget : public QFrame
{
    Q_OBJECT

public:
    explicit sceneWidget(QWidget *parent = 0);
    ~sceneWidget();

    void setPixmap(const QPixmap &);

    void setName( const QString &name );
    QString getName();

    void setRobot( robotBase *pBase );
    robotBase *getRobot();

    void setModelObj( mcModelObj *pBase );
    mcModelObj *getModelObj();

    void setSelected( bool b );
    bool getSelected();

protected:
    robotBase *m_pBase;
    mcModelObj *m_pModel;

    bool mbSelected;
    QLineEdit *mpLabel;

private:
    Ui::sceneWidget *ui;
};

#endif // SCENEWIDGET_H
