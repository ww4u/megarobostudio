#ifndef MRQTRIGCHAN_H
#define MRQTRIGCHAN_H

#include <QWidget>
#include "modelview.h"
namespace Ui {
class MrqTrigChan;
}

class trigLevelConfig
{
public:
    bool mbOnOff;
    int mTypeIndex, mRespIndex;
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
private:
    Ui::MrqTrigChan *ui;
};

#endif // MRQTRIGCHAN_H
