#ifndef ENCODERALARMPAGE_H
#define ENCODERALARMPAGE_H

#include <QWidget>
#include "modelview.h"

namespace Ui {
class EncoderAlarmPage;
}

struct EncoderAlarmConfig
{
    bool mbEn;
    quint32 mUpLimit, mDownLimit;
    quint32 mZero;
    int     mZeroPos;
};

class EncoderAlarmPage : public modelView
{
    Q_OBJECT

public:
    explicit EncoderAlarmPage(QWidget *parent = 0);
    ~EncoderAlarmPage();

public:
    void setData( EncoderAlarmConfig &config );
    void data( EncoderAlarmConfig &config );

protected:
    void spyEdited();
private:
    Ui::EncoderAlarmPage *ui;
};

#endif // ENCODERALARMPAGE_H
