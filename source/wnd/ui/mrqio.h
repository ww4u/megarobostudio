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

protected:
    virtual void changeEvent( QEvent *event );

public:
    virtual int setApply();
public:
    virtual void modelChanged();

protected:
    void spyEdited();
    void setupUi();
    void desetupUi();

    int apply();
    int updateUi();

    void adaptUi();

private:
    Ui::mrqIo *ui;
};

#endif // MRQIO_H
