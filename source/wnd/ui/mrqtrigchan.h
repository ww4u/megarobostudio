#ifndef MRQTRIGCHAN_H
#define MRQTRIGCHAN_H

#include <QWidget>
#include "modelview.h"
//#include "pageoption.h"
namespace Ui {
class MrqTrigChan;
}

class trigLevelConfig
{
public:
    bool mbOnOff;
    int mTypeIndex, mRespIndex;
    int mRunIndex;
    float mPeriod;
};

class trigPatternConfig
{

};

class MrqTrigChan : public modelView
{
    Q_OBJECT

public:
    explicit MrqTrigChan(QWidget *parent = 0);
    ~MrqTrigChan();

protected:
    virtual void changeEvent( QEvent *event );

public:
    void setConfig(
                    trigLevelConfig &levConfig,
                    trigPatternConfig &pattConifg
                    );
    void getConfig(
                    trigLevelConfig &levConfig,
                    trigPatternConfig &pattConifg );
protected:
    void spyEdited();
private slots:
    void on_cmbLevelResponse_currentIndexChanged(int index);

private:
    Ui::MrqTrigChan *ui;
};

#endif // MRQTRIGCHAN_H
