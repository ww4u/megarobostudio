#ifndef MRVVERTICAL_H
#define MRVVERTICAL_H

#include <QWidget>
#include "mrvview.h"
namespace Ui {
class MrvVertical;
}

class MrvVertical : public mrvView
{
    Q_OBJECT

public:
    explicit MrvVertical(QWidget *parent = 0);
    ~MrvVertical();

protected:
    virtual void changeEvent( QEvent *event );

public:
    virtual int setApply();
public:
    virtual void modelChanged();

protected:
    void spyEdited();
//    void setupUi();
//    void desetupUi();

//    void adaptUi();

    int apply();
    int updateUi();

private slots:
    void on_chkLed_clicked(bool checked);

private:
    Ui::MrvVertical *ui;
};

#endif // MRVVERTICAL_H
