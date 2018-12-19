#ifndef TESTPROP_H
#define TESTPROP_H

#include <QWidget>
#include "modelview.h"
#include "mrqview.h"
namespace Ui {
class TestProp;
}

class TestProp : public modelView
{
    Q_OBJECT

public:
    explicit TestProp(QWidget *parent = 0);
    ~TestProp();

protected:
    virtual void changeEvent( QEvent *event );

private:
    Ui::TestProp *ui;
};

#endif // TESTPROP_H
