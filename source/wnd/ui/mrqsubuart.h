#ifndef MRQSUBUART_H
#define MRQSUBUART_H

#include <QWidget>

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

class MrqSubUart : public QWidget
{
    Q_OBJECT

public:
    explicit MrqSubUart(QWidget *parent = 0);
    ~MrqSubUart();

    void setConfig( subUartConfig &cfg );
    void getConfig( subUartConfig &cfg );

private:
    Ui::MrqSubUart *ui;
};

#endif // MRQSUBUART_H
