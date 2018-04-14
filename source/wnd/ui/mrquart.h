#ifndef MRQUART_H
#define MRQUART_H

#include <QWidget>
#include "modelview.h"
namespace Ui {
class MrqUart;
}

class uartConfig
{
public:
    int mBaudInd;
    int mDataInd;
    int mParityInd;
    int mStopInd;
    int mFlowInd;
};

class MrqUart : public modelView
{
    Q_OBJECT

public:
    explicit MrqUart(QWidget *parent = 0);
    ~MrqUart();

    void setConfig(  uartConfig &cfg );
    void getConfig( uartConfig &cfg );
protected:
    void spyEdited();
private:
    Ui::MrqUart *ui;
};

#endif // MRQUART_H
