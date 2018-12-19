#ifndef MRQIN_H
#define MRQIN_H

#include <QtWidgets>
#include "mrqview.h"

namespace Ui {
class MrqIn;
}

class MrqIn : public mrqView
{
    Q_OBJECT

public:
    explicit MrqIn(QWidget *parent = 0);
    ~MrqIn();

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
    Ui::MrqIn *ui;
};

#endif // MRQIN_H
