#ifndef MRQIO_H
#define MRQIO_H

#include <QWidget>
#include "mrqview.h"

namespace Ui {
class mrqIo;
}

class mrqIo : public mrqView
{
    Q_OBJECT

public:
    explicit mrqIo(QWidget *parent = 0);
    ~mrqIo();

public:
    virtual int setApply();
protected:
    virtual void modelChanged();

protected:
    void setupUi();
    void desetupUi();

    int apply();
    int updateData();

private:
    Ui::mrqIo *ui;
};

#endif // MRQIO_H
