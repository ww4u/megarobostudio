#ifndef MRQSUBUART_H
#define MRQSUBUART_H

#include <QWidget>
#include "modelview.h"
namespace Ui {
class MrqSubUart;
}

class subUartConfig
{
public:
    bool mbOnOff;
    int mSof;
    int mLength;
    int mReceiveNum;
    float mInterval;
};

class MrqSubUart : public modelView
{
    Q_OBJECT

public:
    explicit MrqSubUart(QWidget *parent = 0);
    ~MrqSubUart();

    void setConfig( subUartConfig &cfg );
    void getConfig( subUartConfig &cfg );

protected:
    void spyEdited();

private:
    Ui::MrqSubUart *ui;
};

#endif // MRQSUBUART_H
