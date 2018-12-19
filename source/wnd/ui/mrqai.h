#ifndef MRQAI_H
#define MRQAI_H

#include <QWidget>
#include "mrqview.h"

namespace Ui {
class MrqAi;
}

struct AiConfig
{
    bool mbEn;
    float mHT, mLT;
    int mHResp, mLResp;
};

class MrqAi : public mrqView
{
    Q_OBJECT

public:
    explicit MrqAi(QWidget *parent = 0);
    ~MrqAi();

protected:
    virtual void changeEvent( QEvent *event );

public:
    void setConfig( AiConfig &cfg );
    void getConfig( AiConfig &cfg );

protected:
    void setupUi();
    void spyEdited();

private:
    Ui::MrqAi *ui;
};

#endif // MRQAI_H
