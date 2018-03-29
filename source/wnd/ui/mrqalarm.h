#ifndef MRQALARM_H
#define MRQALARM_H

#include <QWidget>

namespace Ui {
class MrqAlarm;
}

class MrqAlarm : public QWidget
{
    Q_OBJECT

public:
    explicit MrqAlarm(QWidget *parent = 0);
    ~MrqAlarm();

private:
    Ui::MrqAlarm *ui;
};

#endif // MRQALARM_H
