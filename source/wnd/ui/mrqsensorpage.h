#ifndef MRQSENSORPAGE_H
#define MRQSENSORPAGE_H

#include <QWidget>
#include "modelview.h"
namespace Ui {
class MrqSensorPage;
}
#include "mrquart.h"
#include "mrqsubuart.h"
class MrqSensorPage : public modelView
{
    Q_OBJECT

public:
    explicit MrqSensorPage(QWidget *parent = 0);
    ~MrqSensorPage();

    void setUartConfig(
                     uartConfig &cfg
                     );
    void getUartConfig(
            uartConfig &cfg
                     );

    void setSubUartConfig(
                     int index,
                     subUartConfig &cfg
                     );
    void getSubUartConfig(
                    int index,
                    subUartConfig &cfg
                     );

    int sensCount();
    void removeSens( int id );

    void setSensText( int id, const QString &tabTxt );

protected:
    void spyEdited();
private slots:
    void on_chkAllEn_clicked(bool checked);
    void slot_sub_live_changed( );

private:
    Ui::MrqSensorPage *ui;

    QList< MrqSubUart * > mSubList;


};

#endif // MRQSENSORPAGE_H
