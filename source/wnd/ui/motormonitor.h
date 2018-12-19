#ifndef MOTORMONITOR_H
#define MOTORMONITOR_H

#include <QtWidgets>

#include "../../com/robomsg.h"
#include "../../model/mcmodel.h"

#include "../../widget/flowlayout.h"

namespace Ui {
class MotorMonitor;
}

class MotorState;
class MotorMonitor : public QDialog
{
    Q_OBJECT

public:
    explicit MotorMonitor(QWidget *parent = 0);
    ~MotorMonitor();

protected:
    virtual void changeEvent( QEvent *event );

public:
    void setModel( mcModel *pModel );

    void setMonitors( const QStringList &conns );
    QStringList monitors();

    void clearMonitors();

    void addMonitor( const QString &str );

protected:
    MotorState *getMotor( const QString &name, int ax );

    void motorStateChanged( const QString &name,
                            int ax,
                            RoboMsg msg );

protected Q_SLOTS:
    void slot_net_event( const QString &name,
                         int axes,
                         RoboMsg msg );

private:
    Ui::MotorMonitor *ui;

    mcModel *m_pModel;

    FlowLayout *m_pFlowLayout;

    QList<MotorState*> mMotors;
    QStringList mMonitors;
};

#endif // MOTORMONITOR_H
