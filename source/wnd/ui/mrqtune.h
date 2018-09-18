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

private:
    Ui::MrqTune *ui;
};

#endif // MRQTUNE_H
