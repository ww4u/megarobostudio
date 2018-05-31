#ifndef MRQTRIGPAGE_H
#define MRQTRIGPAGE_H

#include <QtWidgets>
#include "mrqview.h"
#include "mrqtrigchan.h"
namespace Ui {
class MrqTrigPage;
}

//class MrqTrigChan;

class MrqTrigPage : public mrqView
{
    Q_OBJECT

public:
    explicit MrqTrigPage(QWidget *parent = 0);
    ~MrqTrigPage();

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
    Ui::MrqTrigPage *ui;

    QList< MrqTrigChan *> mTrigChans;

};

#endif // MRQTRIGPAGE_H
