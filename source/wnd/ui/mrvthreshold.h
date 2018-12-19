#ifndef MRVTHRESHOLD_H
#define MRVTHRESHOLD_H

#include <QWidget>
#include "mrvview.h"
namespace Ui {
class MrvThreshold;
}

class MrvThreshold : public mrvView
{
    Q_OBJECT

public:
    explicit MrvThreshold(QWidget *parent = 0);
    ~MrvThreshold();

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

private:
    Ui::MrvThreshold *ui;
};

#endif // MRVTHRESHOLD_H
