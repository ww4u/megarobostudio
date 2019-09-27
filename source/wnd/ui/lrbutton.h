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

protected:
    virtual void changeEvent( QEvent *event );

public:
    void setStep( float stp );
    float step();

    void setSingleStep( float singleStep );

Q_SIGNALS:
    void signal_step( float step );
    void signal_step( float step, bool b );

private slots:
    void on_btnLeft_clicked();
    void on_btnRight_clicked();

    void on_btnLeft_pressed();
    void on_btnLeft_released();

    void on_btnRight_pressed();
    void on_btnRight_released();

private:
    Ui::LRButton *ui;
};

#endif // LRBUTTON_H
