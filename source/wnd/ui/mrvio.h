#ifndef MRVIO_H
#define MRVIO_H

#include <QWidget>
#include "mrvview.h"
namespace Ui {
class MrvIo;
}

class MrvIo : public mrvView
{
    Q_OBJECT

public:
    explicit MrvIo(QWidget *parent = 0);
    ~MrvIo();

public:
    virtual int setApply();
public:
    virtual void modelChanged();

protected:
    void spyEdited();

    int apply();
    int updateUi();

private:
    Ui::MrvIo *ui;
};

#endif // MRVIO_H
