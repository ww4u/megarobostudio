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

public:
    virtual int setApply();
protected:
    virtual void modelChanged();

protected:
    int apply();
    int updateUi();
    int updateData();


private:
    Ui::MrqAxesPlan *ui;
};

#endif // MRQAXESPLAN_H
