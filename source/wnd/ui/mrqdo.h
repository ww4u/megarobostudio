#ifndef MRQDO_H
#define MRQDO_H

#include <QWidget>
#include "mrqview.h"

namespace Ui {
class MrqDO;
}

struct DoConfig
{
    bool mbEn;

    int mSrcIndex;
    int mSignalIndex;
    int mPolarityIndex;
    int mCondIndex;

    float mPeri, mDuty;
};

class MrqDO : public mrqView
{
    Q_OBJECT

public:
    explicit MrqDO(QWidget *parent = 0);
    ~MrqDO();

    void setConfig( DoConfig &cfg );
    void getConfig( DoConfig &cfg );

protected:
    void setupUi();

    void spyEdited();

private:
    Ui::MrqDO *ui;
};

#endif // MRQDO_H
