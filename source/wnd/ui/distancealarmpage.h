#ifndef DISTANCEALARMPAGE_H
#define DISTANCEALARMPAGE_H

#include <QWidget>

namespace Ui {
class DistanceAlarmPage;
}

class DistanceAlarmPage : public QWidget
{
    Q_OBJECT

public:
    explicit DistanceAlarmPage(QWidget *parent = 0);
    ~DistanceAlarmPage();

private:
    Ui::DistanceAlarmPage *ui;
};

#endif // DISTANCEALARMPAGE_H
