#ifndef MRVSWITCH_H
#define MRVSWITCH_H

#include <QWidget>
#include "modelview.h"
namespace Ui {
class MrvSwitch;
}

struct switchConfig{
    quint16 mFilterDelay;
    quint16 mFilterCount;
    int     mTrigEdge;
};

class MrvSwitch : public modelView
{
    Q_OBJECT

public:
    explicit MrvSwitch(QWidget *parent = 0);
    ~MrvSwitch();

protected:
    void spyEdited();

public:
    void setConfig( switchConfig &cfg );
    void getConifig( switchConfig &cfg );

private:
    Ui::MrvSwitch *ui;
};

#endif // MRVSWITCH_H
