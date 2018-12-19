#ifndef ROBOCONFIG_H
#define ROBOCONFIG_H

#include <QWidget>

namespace Ui {
class RoboConfig;
}

class RoboConfig : public QWidget
{
    Q_OBJECT

public:
    explicit RoboConfig(QWidget *parent = 0);
    ~RoboConfig();

protected:
    virtual void changeEvent( QEvent *event );

private:
    Ui::RoboConfig *ui;
};

#endif // ROBOCONFIG_H
