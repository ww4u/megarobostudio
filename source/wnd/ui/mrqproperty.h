#ifndef MRQPROPERTY_H
#define MRQPROPERTY_H

#include <QtWidgets>

#include "mrqinfo.h"
#include "mrqaxespage.h"
//#include "mrqaxes.h"
//#include "mrqaxes2.h"
//#include "mrqaxesplan.h"
//#include "mrqtrigpage.h"


#include "mrqio.h"
#include "mrqsensor.h"
#include "mrqsys.h"

#include "mrqdcaxes.h"
#include "mrqalarm.h"

#include "../../model/mcmodelobj.h"

namespace Ui {
class mrqProperty;
}

class mrqProperty : public mrqView
{
    Q_OBJECT

public:
    explicit mrqProperty( VRobot *pMrqRobo,
                          QWidget *parent = 0);
    ~mrqProperty();

protected Q_SLOTS:
    void slot_page_changed( int index );
    void on_btnApply_clicked();
    void on_btnOK_clicked();
    void on_btnCancel_clicked();

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual int  save( QString &outFileName );
    virtual int  saveAs( QString &outFileName );

    virtual void setMcModel( mcModel *pMcModel );

    virtual int setApply();

protected:
    void setupUi( );
    void desetupUi();

    void buildConnection();

private:
    Ui::mrqProperty *ui;

public:
//    int mAxes;
    VRobot *m_pRefModel;

    mrqInfo *m_pInfoPage;

    QList< MrqAxesPage * > mMrqAxesPages;
//    QList< mrqAxes * > mAxesPages;
//    QList< mrqAxes2 * > mAxesPage2s;
//    QList< MrqTrigPage * > mAxesPage3s;
//    QList< MrqAxesPlan *> mAxesPlans;

    QList< MrqDcAxes *> mDcAxesPages;

    mrqIo *m_pIoPage;
    mrqSensor *m_pSensorPage;
    mrqSys *m_pSysPage;
    MrqAlarm *m_pAlarmPage;
};

#endif // MRQPROPERTY_H
