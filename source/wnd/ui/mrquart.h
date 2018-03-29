#ifndef MRQUART_H
#define MRQUART_H

#include <QWidget>

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

class MrqUart : public QWidget
{
    Q_OBJECT

public:
    explicit MrqUart(QWidget *parent = 0);
    ~MrqUart();

    void setConfig(  uartConfig &cfg );

    void getConfig( uartConfig &cfg );

private:
    Ui::MrqUart *ui;
};

#endif // MRQUART_H
