#ifndef MRQTUNE_H
#define MRQTUNE_H

#include <QWidget>
#include "mrqview.h"
namespace Ui {
class MrqTune;
}

class MrqTune : public mrqView
{
    Q_OBJECT

public:
    explicit MrqTune(QWidget *parent = 0);
    ~MrqTune();

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

private slots:
    void on_spinUp_valueChanged(double arg1);

    void on_spinDown_valueChanged(double arg1);

private:
    Ui::MrqTune *ui;
};

#endif // MRQTUNE_H
