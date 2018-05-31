#ifndef MRQYO_H
#define MRQYO_H

#include <QWidget>
#include "mrqview.h"
namespace Ui {
class MrqYO;
}

struct YoConfig
{
    bool mbEn;
    int mSrcIndex;
    int mCondIndex;
    int mRespIndex;
};

class MrqYO : public mrqView
{
    Q_OBJECT

public:
    explicit MrqYO(QWidget *parent = 0);
    ~MrqYO();

    void setConfig( YoConfig &cfg );
    void getConfig( YoConfig &cfg );

protected:
    void setupUi();

    void spyEdited();

private:
    Ui::MrqYO *ui;
};

#endif // MRQYO_H
