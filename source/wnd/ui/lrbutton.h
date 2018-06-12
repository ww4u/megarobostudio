#ifndef LRBUTTON_H
#define LRBUTTON_H

#include <QWidget>

namespace Ui {
class LRButton;
}

class LRButton : public QWidget
{
    Q_OBJECT

public:
    explicit LRButton(QWidget *parent = 0);
    ~LRButton();

    void setStep( float stp );
    float step();

Q_SIGNALS:
    void signal_step( float step );

private slots:
    void on_btnLeft_clicked();

    void on_btnRight_clicked();

private:
    Ui::LRButton *ui;
};

#endif // LRBUTTON_H
