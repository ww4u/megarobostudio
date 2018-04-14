#ifndef DISTANCEALARMPAGE_H
#define DISTANCEALARMPAGE_H

#include <QWidget>
#include "modelview.h"
namespace Ui {
class DistanceAlarmPage;
}

struct DistAlarmConfig
{
    bool mbEn;
    quint16 mDist[3];
};

class DistanceAlarmPage : public modelView
{
    Q_OBJECT

public:
    explicit DistanceAlarmPage(QWidget *parent = 0);
    ~DistanceAlarmPage();

public:
    void setData( DistAlarmConfig &config );
    void data( DistAlarmConfig &config );

protected:
    void spyEdited();
private:
    Ui::DistanceAlarmPage *ui;
};

#endif // DISTANCEALARMPAGE_H
