#ifndef MRQALARM_H
#define MRQALARM_H

#include <QWidget>
#include "mrqview.h"

#include "distancealarmpage.h"
#include "encoderalarmpage.h"

namespace Ui {
class MrqAlarm;
}

class MrqAlarm : public mrqView
{
    Q_OBJECT

public:
    explicit MrqAlarm(QWidget *parent = 0);
    ~MrqAlarm();

protected:
    virtual void changeEvent( QEvent *event );

public:
    virtual int setApply();
public:
    virtual void modelChanged();

protected:
    void spyEdited();

    void updateUi();
    void updateData();

    int applyAngleAlarm();
    int applyDistanceAlarm();

private:
    Ui::MrqAlarm *ui;

    QList<EncoderAlarmPage *> mAnglePages;
    QList<DistanceAlarmPage *> mDistPages;
};

#endif // MRQALARM_H
