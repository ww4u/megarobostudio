#ifndef MRQAXESPLAN_H
#define MRQAXESPLAN_H

#include <QWidget>
#include "mrqview.h"
namespace Ui {
class MrqAxesPlan;
}

class MrqAxesPlan : public mrqView
{
    Q_OBJECT

public:
    explicit MrqAxesPlan(QWidget *parent = 0);
    ~MrqAxesPlan();

protected:
    virtual void changeEvent( QEvent *event );

public:
    virtual int setApply();
public:
    virtual void modelChanged();

protected:
    void spyEdited();
    int apply();
    int updateUi();
    int updateData();

private:
    Ui::MrqAxesPlan *ui;
};

#endif // MRQAXESPLAN_H
