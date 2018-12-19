#ifndef MRQXI_H
#define MRQXI_H

#include <QWidget>
#include "mrqview.h"
namespace Ui {
class MrqXI;
}

struct XiConfig
{
    bool mbEn;
    int mTypeIndex;
    int mRespIndex;
    int mRespCH;
    float mPeri;
};

class MrqXI : public mrqView
{
    Q_OBJECT

public:
    explicit MrqXI(QWidget *parent = 0);
    ~MrqXI();

protected:
    virtual void changeEvent( QEvent *event );

public:
    void setConfig( XiConfig &cfg );
    void getConfig( XiConfig &cfg );

protected:
    void setupUi();
    void spyEdited();

private:
    Ui::MrqXI *ui;
};

#endif // MRQXI_H
